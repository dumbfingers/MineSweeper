#include <SPI.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include "GameState.h"
#include "MInefield.h"
#include "Cursor.h"
#include "Dialog.h"
#include "Sound.h"
#include "Graphics.h"

Display *display;
Sprite *sprite;

bool confirm;
bool shouldShowMenu;
bool buttonAPressed;

int TOTAL_MINES = 10;
int revealedCount = 0;

GameState gameState;
Minefield board[100][100];
Cursor cursor;

void setup()
{
    Nibble.begin();
    display = Nibble.getDisplay();
    sprite = display->getBaseSprite();
    Input::getInstance()->setBtnPressCallback(BTN_UP, BTN_UP_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_UP, BTN_UP_release);
    Input::getInstance()->setButtonHeldCallback(BTN_UP, 300, BTN_UP_held_300ms);
    Input::getInstance()->setBtnPressCallback(BTN_RIGHT, BTN_RIGHT_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_RIGHT, BTN_RIGHT_release);
    Input::getInstance()->setButtonHeldCallback(BTN_RIGHT, 300, BTN_RIGHT_held_300ms);
    Input::getInstance()->setBtnPressCallback(BTN_LEFT, BTN_LEFT_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_LEFT, BTN_LEFT_release);
    Input::getInstance()->setButtonHeldCallback(BTN_LEFT, 300, BTN_LEFT_held_300ms);
    Input::getInstance()->setBtnPressCallback(BTN_DOWN, BTN_DOWN_press);
    Input::getInstance()->setBtnPressCallback(BTN_A, BTN_A_press);
    Input::getInstance()->setBtnPressCallback(BTN_B, BTN_B_press);
    Input::getInstance()->setBtnPressCallback(BTN_C, BTN_C_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_C, BTN_C_release);

    sprite->setTextSize(1);
    sprite->setTextFont(2);
    sprite->setTextColor(TFT_BLACK);
    sprite->clear(TFT_BLACK);

    initBoard();
}

void loop()
{
    Input::getInstance()->loop(0);

    if (shouldShowMenu)
    {
        showMenu();
    }

    display->commit();
}

void showMenu()
{
    sprite->fillRect(20, 20, 88, 108, TFT_LIGHTGREY);
    sprite->drawRect(20, 20, 88, 108, TFT_BLACK);
    sprite->setCursor(26, 34);
    sprite->print("Wanna quit?");
    sprite->fillRect(38, 72, 52, 20, TFT_DARKGREY);
    sprite->drawRect(38, 72, 52, 20, TFT_BLACK);
    sprite->setCursor(48, 72);
    sprite->print("YES");
    sprite->fillRect(38, 98, 52, 20, TFT_DARKGREY);
    sprite->drawRect(38, 98, 52, 20, TFT_BLACK);
    sprite->setCursor(48, 98);
    sprite->print("NO");
}

void initBoard()
{
    // Top board: mines count and timer
    sprite->fillRect(0, 0, 128, 20, TFT_LIGHTGREY);
    sprite->drawLine(0, 20, 128, 20, TFT_DARKGREY);

    sprite->setCursor(14, 4);
    sprite->print(TOTAL_MINES);
    sprite->setCursor(80, 4);
    sprite->print("0");

    // Main board: draw mines, from (14, 24) to (114, 24)
    // board = 100 x 100 px
    // each mine is 6x6, with 2px gap on top, bot, left and right
    sprite->fillRect(0, 20, 128, 108, TFT_LIGHTGREY);

    int start_x = 14;
    int start_y = 24;
    int mine_dimen = 6;
    int gap = 2;

    for (int i = 1; i <= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            sprite->fillRect(
                start_x + gap + (mine_dimen + gap * 2) * (i - 1),
                start_y + gap + (mine_dimen + gap * 2) * (j - 1),
                mine_dimen,
                mine_dimen,
                TFT_DARKGREY);

            // draw shadow on right and bottom
            sprite->drawLine(
                start_x + gap + mine_dimen + (mine_dimen + gap * 2) * (i - 1),
                start_y + gap + (mine_dimen + gap * 2) * (j - 1),
                start_x + gap + mine_dimen + (mine_dimen + gap * 2) * (i - 1),
                start_y + gap + mine_dimen + (mine_dimen + gap * 2) * (j - 1),
                TFT_BLACK);
            sprite->drawLine(
                start_x + gap + (mine_dimen + gap * 2) * (i - 1),
                start_y + gap + mine_dimen + (mine_dimen + gap * 2) * (j - 1),
                start_x + gap + mine_dimen + (mine_dimen + gap * 2) * (i - 1),
                start_y + gap + mine_dimen + (mine_dimen + gap * 2) * (j - 1),
                TFT_BLACK);
        }
    }

    // TEST
    // for (int i = 1; i <= 8; i++) {
    //     drawDigit(start_x + (i - 1) * 10, start_y, i);
    // }
}

void placeMines()
{
    int i = 0;
    int x = 0;
    int y = 0;
    while (i < TOTAL_MINES)
    {
        x = random(1, 10);
        y = random(1, 10);

        if (!board[x][y].isMine &&
            !(x == cursor.x + 1 && y == cursor.y + 1))
        {
            board[x][y].isMine = true;
            i++;
        }
    }
}

void updateHint()
{
    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
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
    if (x > 0 && y > 0 && x <= 10 && y <= 10)
    {
        Minefield location = board[x][y];
        if (location.state == COVERED)
        {
            if (!location.isMine)
            {
                location.state = REVEALED;
                revealedCount++;

                if (location.nearByMines == 0)
                {
                    revealNeighbours(x + 1, y + 1);
                    revealNeighbours(x + 1, y);
                    revealNeighbours(x + 1, y - 1);

                    revealNeighbours(x, y + 1);
                    revealNeighbours(x, y - 1);

                    revealNeighbours(x - 1, y + 1);
                    revealNeighbours(x - 1, y);
                    revealNeighbours(x - 1, y - 1);
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
    if (gameState == LOST)
    {
        return;
    }
    buttonAPressed = false;
    int x = cursor.x;
    int y = cursor.y;
    buttonAPressed = false;
    Minefield location = board[cursor.x][cursor.y];
    if (location.state == COVERED)
    {
        if (location.isMine)
        {
            gameState = LOST;
            // reveal all mines
            for (int i = 1; i < 10; i++)
            {
                for (int j = 1; j < 10; j++)
                {
                    if (board[i][j].isMine)
                    {
                        board[i][j].state = REVEALED;
                    }
                }
            }
            // TODO:
            // play lost sound
            playCancel();
            // show dialog
            showDialog("You Lost", "Restart", "Cancel");
        }
        else
        {
            playOk();
            if (location.nearByMines == 0)
            {
                revealNeighbours(cursor.x, cursor.y);
            } else
            {
                location.state = REVEALED;
                revealedCount++;
            }

            if (revealedCount == 100 - TOTAL_MINES)
            {
                gameState = WON;
                playWon();
                // TODO: more options after winning the game
                showDialog("You WIN!!!", "OK", "Cancel");
            }
            
            
        }
    }
    
    
}

void BTN_B_press()
{
    if (Piezo.isMuted())
    {
        Piezo.setMute(false);
    }
    else
    {
        Piezo.setMute(true);
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
}

void BTN_UP_press()
{
}

void BTN_UP_release()
{
}

void BTN_UP_held_300ms()
{
}

void BTN_RIGHT_press()
{
}

void BTN_RIGHT_release()
{
}

void BTN_RIGHT_held_300ms()
{
} 

void BTN_LEFT_press()
{
}

void BTN_LEFT_release()
{
}

void BTN_LEFT_held_300ms()
{
}
#pragma endregion