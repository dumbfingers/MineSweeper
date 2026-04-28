#include <SPI.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include "GameState.h"
#include "Minefield.h"
#include "Cursor.h"
#include "Dialog.h"
#include "Sound.h"
#include "Graphics.h"

Display *display;
Sprite *sprite;

bool confirm;
bool shouldShowMenu;
bool buttonAPressed;

const int BOARD_SIZE = 10;
int TOTAL_MINES = 10;
int revealedCount = 0;

GameState gameState;
Minefield board[BOARD_SIZE + 2][BOARD_SIZE + 2];
Cursor cursor;

void setup()
{
    Nibble.begin();
    display = Nibble.getDisplay();
    sprite = display->getBaseSprite();
    Input::getInstance()->setBtnPressCallback(BTN_UP, BTN_UP_press);
    Input::getInstance()->setBtnPressCallback(BTN_RIGHT, BTN_RIGHT_press);
    Input::getInstance()->setBtnPressCallback(BTN_LEFT, BTN_LEFT_press);
    Input::getInstance()->setBtnPressCallback(BTN_DOWN, BTN_DOWN_press);
    Input::getInstance()->setBtnPressCallback(BTN_A, BTN_A_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_A, BTN_A_release);
    Input::getInstance()->setBtnPressCallback(BTN_B, BTN_B_press);
    Input::getInstance()->setBtnPressCallback(BTN_C, BTN_C_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_C, BTN_C_release);

    sprite->setTextSize(1);
    sprite->setTextFont(2);
    sprite->setTextColor(TFT_BLACK);

    newGame();
}

void loop()
{
    Input::getInstance()->loop(0);

    draw();
}

void draw()
{
    sprite->clear(TFT_BLACK);
    drawBoard();
    if (gameState == LOST)
    {
        showDialog("You Lost", "Restart", "C:Menu");
    }
    else if (gameState == WON)
    {
        showDialog("You WIN!!!", "OK", "C:Menu");
    }

    if (shouldShowMenu)
    {
        showMenu();
    }

    display->commit();
}

void newGame()
{
    gameState = RUNNING;
    revealedCount = 0;
    for (int i = 0; i < BOARD_SIZE + 2; i++)
    {
        for (int j = 0; j < BOARD_SIZE + 2; j++)
        {
            board[i][j].isMine = false;
            board[i][j].nearByMines = 0;
            board[i][j].state = COVERED;
        }
    }
    cursor.x = BOARD_SIZE / 2;
    cursor.y = BOARD_SIZE / 2;
    placeMines();
    updateHint();
}

void showMenu()
{
    sprite->fillRect(10, 30, 108, 68, TFT_LIGHTGREY);
    sprite->drawRect(10, 30, 108, 68, TFT_BLACK);
    sprite->setCursor(20, 40);
    sprite->print("MineSweeper");
    sprite->setCursor(20, 60);
    sprite->print("A: Reveal/New");
    sprite->setCursor(20, 75);
    sprite->print("B: Flag/Unflag");
}

void drawBoard()
{
    // Top board: mines count and timer
    sprite->fillRect(0, 0, 128, 20, TFT_LIGHTGREY);
    sprite->drawLine(0, 20, 128, 20, TFT_DARKGREY);

    int flaggedCount = 0;
    for (int i = 1; i <= BOARD_SIZE; i++)
        for (int j = 1; j <= BOARD_SIZE; j++)
            if (board[i][j].state == FLAGGED) flaggedCount++;

    sprite->setCursor(14, 4);
    sprite->print(TOTAL_MINES - flaggedCount);
    sprite->setCursor(80, 4);
    sprite->print("0"); // TODO: Timer

    // Main board: 128x108 space
    sprite->fillRect(0, 20, 128, 108, TFT_LIGHTGREY);

    int start_x = 14;
    int start_y = 24;
    int cell_size = 10;
    int mine_dimen = 6;
    int gap = 2;

    for (int i = 1; i <= BOARD_SIZE; i++)
    {
        for (int j = 1; j <= BOARD_SIZE; j++)
        {
            int x = start_x + (i - 1) * cell_size;
            int y = start_y + (j - 1) * cell_size;

            if (board[i][j].state == REVEALED)
            {
                if (board[i][j].isMine)
                {
                    sprite->fillCircle(x + 5, y + 5, 3, TFT_BLACK);
                }
                else if (board[i][j].nearByMines > 0)
                {
                    drawDigit(x, y, board[i][j].nearByMines);
                }
            }
            else
            {
                // Covered or Flagged
                sprite->fillRect(x + 1, y + 1, cell_size - 2, cell_size - 2, TFT_DARKGREY);
                if (board[i][j].state == FLAGGED)
                {
                    sprite->fillTriangle(x + 3, y + 2, x + 3, y + 5, x + 7, y + 3.5, TFT_RED);
                    sprite->drawLine(x + 3, y + 2, x + 3, y + 8, TFT_BLACK);
                }

                // Draw shadow for 3D effect
                sprite->drawLine(x + 9, y, x + 9, y + 9, TFT_BLACK);
                sprite->drawLine(x, y + 9, x + 9, y + 9, TFT_BLACK);
                sprite->drawLine(x, y, x + 9, y, TFT_WHITE);
                sprite->drawLine(x, y, x, y + 9, TFT_WHITE);
            }

            // Draw cursor
            if (cursor.x == i && cursor.y == j)
            {
                sprite->drawRect(x, y, cell_size, cell_size, TFT_YELLOW);
                sprite->drawRect(x + 1, y + 1, cell_size - 2, cell_size - 2, TFT_YELLOW);
            }
        }
    }
}

void placeMines()
{
    int i = 0;
    int x = 0;
    int y = 0;
    while (i < TOTAL_MINES)
    {
        x = random(1, BOARD_SIZE + 1);
        y = random(1, BOARD_SIZE + 1);

        if (!board[x][y].isMine &&
            !(x == cursor.x && y == cursor.y))
        {
            board[x][y].isMine = true;
            i++;
        }
    }
}

void updateHint()
{
    for (int i = 1; i <= BOARD_SIZE; i++)
    {
        for (int j = 1; j <= BOARD_SIZE; j++)
        {
            if (!board[i][j].isMine)
            {
                board[i][j].nearByMines = getNearByMineCount(i, j);
            }
        }
    }
}

int getNearByMineCount(int x, int y)
{
    int count = 0;

    if (board[x][y - 1].isMine)
        count++;
    if (board[x][y + 1].isMine)
        count++;

    if (board[x - 1][y - 1].isMine)
        count++;
    if (board[x - 1][y].isMine)
        count++;
    if (board[x - 1][y + 1].isMine)
        count++;

    if (board[x + 1][y - 1].isMine)
        count++;
    if (board[x + 1][y].isMine)
        count++;
    if (board[x + 1][y + 1].isMine)
        count++;

    return count;
}

void updateBoard()
{
    if (gameState == LOST)
    {
        // show dialog to restart
        showDialog("You Lost", "Restart", "Cancel");
    }
    else if (buttonAPressed)
    {
        buttonAPressed = false;
    }
}

void revealNeighbours(int x, int y)
{
    if (x > 0 && y > 0 && x <= BOARD_SIZE && y <= BOARD_SIZE)
    {
        if (board[x][y].state == COVERED && !board[x][y].isMine)
        {
            board[x][y].state = REVEALED;
            revealedCount++;

            if (board[x][y].nearByMines == 0)
            {
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        if (i == 0 && j == 0) continue;
                        revealNeighbours(x + i, y + j);
                    }
                }
            }
        }
    }
}

#pragma region Buttons

void BTN_A_press()
{
    Piezo.tone(220, 50);
    confirm = true;
}

void BTN_A_release()
{
    if (gameState != RUNNING)
    {
        newGame();
        return;
    }

    if (board[cursor.x][cursor.y].state == COVERED)
    {
        if (board[cursor.x][cursor.y].isMine)
        {
            gameState = LOST;
            // reveal all mines
            for (int i = 1; i <= BOARD_SIZE; i++)
            {
                for (int j = 1; j <= BOARD_SIZE; j++)
                {
                    if (board[i][j].isMine)
                    {
                        board[i][j].state = REVEALED;
                    }
                }
            }
            playCancel();
        }
        else
        {
            playOk();
            if (board[cursor.x][cursor.y].nearByMines == 0)
            {
                revealNeighbours(cursor.x, cursor.y);
            }
            else
            {
                board[cursor.x][cursor.y].state = REVEALED;
                revealedCount++;
            }

            if (revealedCount == BOARD_SIZE * BOARD_SIZE - TOTAL_MINES)
            {
                gameState = WON;
                playWon();
            }
        }
    }
}

void BTN_B_press()
{
    if (gameState != RUNNING) return;

    if (board[cursor.x][cursor.y].state == COVERED)
    {
        board[cursor.x][cursor.y].state = FLAGGED;
        playTick();
    }
    else if (board[cursor.x][cursor.y].state == FLAGGED)
    {
        board[cursor.x][cursor.y].state = COVERED;
        playTick();
    }
}

void BTN_C_press()
{
    shouldShowMenu = true;
}

void BTN_C_release()
{
    shouldShowMenu = false;
}

void BTN_DOWN_press()
{
    if (gameState != RUNNING) return;
    if (cursor.y < BOARD_SIZE) cursor.y++;
}

void BTN_UP_press()
{
    if (gameState != RUNNING) return;
    if (cursor.y > 1) cursor.y--;
}

void BTN_RIGHT_press()
{
    if (gameState != RUNNING) return;
    if (cursor.x < BOARD_SIZE) cursor.x++;
}

void BTN_LEFT_press()
{
    if (gameState != RUNNING) return;
    if (cursor.x > 1) cursor.x--;
}
#pragma endregion