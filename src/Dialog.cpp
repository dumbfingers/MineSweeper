#include "Dialog.h"

void showDialog(String message, String positive, String negative)
{
    sprite->fillRect(20, 20, 88, 108, TFT_LIGHTGREY);
    sprite->drawRect(20, 20, 88, 108, TFT_BLACK);
    sprite->setCursor(26, 34);
    sprite->print(message);
    sprite->fillRect(38, 72, 52, 20, TFT_DARKGREY);
    sprite->drawRect(38, 72, 52, 20, TFT_BLACK);
    sprite->setCursor(48, 72);
    sprite->print(positive);
    sprite->fillRect(38, 98, 52, 20, TFT_DARKGREY);
    sprite->drawRect(38, 98, 52, 20, TFT_BLACK);
    sprite->setCursor(48, 98);
    sprite->print(negative);
}