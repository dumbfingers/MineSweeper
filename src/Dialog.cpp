#include "Dialog.h"
#include <Display/Display.h>

void showDialog(String message, String positive, String negative)
{
    extern Sprite* sprite;
    sprite->fillRect(10, 30, 108, 68, TFT_LIGHTGREY);
    sprite->drawRect(10, 30, 108, 68, TFT_BLACK);
    sprite->setCursor(20, 40);
    sprite->print(message);

    sprite->fillRect(15, 60, 48, 30, TFT_DARKGREY);
    sprite->drawRect(15, 60, 48, 30, TFT_BLACK);
    sprite->setCursor(18, 65);
    sprite->print(positive);

    sprite->fillRect(65, 60, 48, 30, TFT_DARKGREY);
    sprite->drawRect(65, 60, 48, 30, TFT_BLACK);
    sprite->setCursor(68, 65);
    sprite->print(negative);
}