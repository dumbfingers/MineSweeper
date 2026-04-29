#include "Sound.h"

void playTick()
{
    Piezo.tone(1000, 100);
}

void playOk()
{
    Piezo.tone(2000, 200);
}

void playWon()
{
    Piezo.tone(1000, 100);
    delay(100);
    Piezo.tone(1500, 100);
    delay(100);
    Piezo.tone(2000, 300);
}

void playCancel()
{
    Piezo.tone(1500, 300);
}