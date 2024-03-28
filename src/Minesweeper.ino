#include <SPI.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include "GameState.h"

Display *display;
Sprite *sprite;

bool confirm;
bool shouldShowMenu;

const String TOTAL_MINES = "10";

GameState gameState;

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
        }
    }

    // TEST
    for (int i = 1; i <= 8; i++) {
        drawDigit(start_x + (i - 1) * 10, start_y, i);
    }
}

// draw digits within 6x6 square
void drawDigit(int x, int y, int digit)
{   
    int gap = 2;
    x = x + gap;
    y = y + gap;

    switch (digit)
    {
    case 1:
        sprite->drawLine(x + 2, y + 2, x + 3, y + 1, TFT_SKYBLUE);
        sprite->drawLine(x + 3, y + 1, x + 3, y + 5, TFT_SKYBLUE);
        sprite->drawLine(x + 2, y + 5, x + 4, y + 5, TFT_SKYBLUE);
        /* code */
        break;
    case 2:
        sprite->drawLine(x + 2, y + 2, x + 3, y + 1, TFT_GREEN);
        sprite->drawLine(x + 3, y + 1, x + 4, y + 1, TFT_GREEN);
        sprite->drawLine(x + 4, y + 1, x + 5, y + 2, TFT_GREEN);
        sprite->drawLine(x + 5, y + 2, x + 2, y + 6, TFT_GREEN);
        sprite->drawLine(x + 2, y + 6, x + 5, y + 6, TFT_GREEN);
        break;
    case 3:
        sprite->drawLine(x + 2, y + 2, x + 2, y + 1, TFT_RED);
        sprite->drawLine(x + 2, y + 1, x + 4, y + 1, TFT_RED);
        sprite->drawLine(x + 4, y + 1, x + 5, y + 2, TFT_RED);
        sprite->drawLine(x + 5, y + 2, x + 4, y + 3, TFT_RED);
        sprite->drawLine(x + 4, y + 3, x + 5, y + 4, TFT_RED);
        sprite->drawLine(x + 5, y + 4, x + 5, y + 5, TFT_RED);
        sprite->drawLine(x + 5, y + 5, x + 4, y + 6, TFT_RED);
        sprite->drawLine(x + 4, y + 6, x + 2, y + 6, TFT_RED);
        sprite->drawLine(x + 2, y + 6, x + 2, y + 5, TFT_RED);
        break;
    case 4:
        sprite->drawLine(x + 4, y + 1, x + 1, y + 4, TFT_BLUE);
        sprite->drawLine(x + 1, y + 4, x + 5, y + 4, TFT_BLUE);
        sprite->drawLine(x + 4, y + 1, x + 4, y + 6, TFT_BLUE);
        break;
    case 5:
        sprite->drawLine(x + 5, y + 2, x + 5, y + 1, TFT_BROWN);
        sprite->drawLine(x + 5, y + 1, x + 2, y + 1, TFT_BROWN);
        sprite->drawLine(x + 2, y + 1, x + 2, y + 3, TFT_BROWN);
        sprite->drawLine(x + 2, y + 3, x + 4, y + 3, TFT_BROWN);
        sprite->drawLine(x + 4, y + 3, x + 5, y + 4, TFT_BROWN);
        sprite->drawLine(x + 5, y + 4, x + 5, y + 5, TFT_BROWN);
        sprite->drawLine(x + 5, y + 5, x + 4, y + 6, TFT_BROWN);
        sprite->drawLine(x + 4, y + 6, x + 2, y + 6, TFT_BROWN);

        break;
    case 6:
        sprite->drawLine(x + 5, y + 2, x + 4, y + 1, TFT_CYAN);
        sprite->drawLine(x + 4, y + 1, x + 3, y + 1, TFT_CYAN);
        sprite->drawLine(x + 3, y + 1, x + 2, y + 2, TFT_CYAN);
        sprite->drawLine(x + 2, y + 2, x + 2, y + 5, TFT_CYAN);
        sprite->drawLine(x + 2, y + 5, x + 3, y + 6, TFT_CYAN);
        sprite->drawLine(x + 3, y + 6, x + 4, y + 6, TFT_CYAN);
        sprite->drawLine(x + 4, y + 6, x + 5, y + 5, TFT_CYAN);
        sprite->drawLine(x + 5, y + 5, x + 5, y + 4, TFT_CYAN);
        sprite->drawLine(x + 5, y + 4, x + 2, y + 3, TFT_CYAN);
        break;
    case 7:
        sprite->drawLine(x + 2, y + 2, x + 2, y + 1, TFT_BLACK);
        sprite->drawLine(x + 2, y + 1, x + 5, y + 1, TFT_BLACK);
        sprite->drawLine(x + 5, y + 1, x + 3, y + 6, TFT_BLACK);

        break;
    case 8:
        sprite->drawLine(x + 2, y + 2, x + 1, y + 1, TFT_SILVER);
        sprite->drawLine(x + 3, y + 1, x + 4, y + 1, TFT_SILVER);
        sprite->drawLine(x + 4, y + 1, x + 5, y + 2, TFT_SILVER);
        sprite->drawLine(x + 5, y + 2, x + 4, y + 3, TFT_SILVER);
        sprite->drawLine(x + 4, y + 3, x + 5, y + 4, TFT_SILVER);
        sprite->drawLine(x + 5, y + 4, x + 5, y + 5, TFT_SILVER);
        sprite->drawLine(x + 5, y + 5, x + 4, y + 6, TFT_SILVER);
        sprite->drawLine(x + 4, y + 6, x + 3, y + 6, TFT_SILVER);
        sprite->drawLine(x + 3, y + 6, x + 2, y + 5, TFT_SILVER);
        sprite->drawLine(x + 2, y + 5, x + 2, y + 4, TFT_SILVER);
        sprite->drawLine(x + 2, y + 4, x + 3, y + 3, TFT_SILVER);
        sprite->drawLine(x + 3, y + 3, x + 2, y + 2, TFT_SILVER);
        break;
    default:
        break;
    }
}