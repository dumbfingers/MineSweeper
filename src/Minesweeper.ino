#include <SPI.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include "GameState.h"

Display *display;
Sprite *sprite;

bool confirm;
bool shouldShowMenu;

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