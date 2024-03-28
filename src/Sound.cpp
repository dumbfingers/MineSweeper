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
    //TODO
}

void playCancel()
{
    Piezo.tone(1500, 300);
}