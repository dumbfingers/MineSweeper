#include "Graphics.h"


// draw digits within 6x6 square
void drawDigit(int x, int y, int digit)
{
    extern Sprite *sprite;
    
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