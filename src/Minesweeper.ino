#include <SPI.h>
#include <CircuitOS.h>
#include <Nibble.h>

//-------------------------------------

Display *display;
Sprite *sprite;

//-------------------------------------

boolean left_pressed;
boolean right_pressed;
boolean up_pressed;
boolean down_pressed;
boolean a_pressed;
boolean b_pressed;
boolean c_pressed;

int LCDHEIGHT;
int LCDWIDTH;

byte fontx = 5;
byte fonty = 5;

const byte FIELD_WIDTH = fontx + 2;
const byte FIELD_HEIGHT = fonty + 2;

const byte COLUMNS = 20;
const byte ROWS = 15;
const byte BOMB_COUNT = ((COLUMNS + ROWS) / 2) + (ROWS / 2);

const byte WIDTH = COLUMNS + 2;
const byte HEIGHT = ROWS + 2;

const byte offset_x = 2;
const byte offset_y = 2;

const char text[10] = {' ', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

byte uncovered_fields;
byte flag_count;

byte first_field;

boolean NotFirstPress = false;

//-------------------------------------
// board
enum field_state
{
    COVERED,
    UNCOVERED,
    FLAGGED
};

typedef struct
{
    byte is_bomb;
    byte nearby_bombs;
    field_state state;
} minefield;
minefield board[WIDTH][HEIGHT];

//-------------------------------------
// cursor
typedef struct
{
    byte x;
    byte y;
} Cursor;

Cursor cursor;

//-------------------------------------
// game state
enum Game_state
{
    RUNNING,
    WON,
    LOST
};
Game_state game_state;

//-------------------------------------
//define special characters (flag, neutral face)
const byte flag[] PROGMEM =
    {
        8,
        7,
        B10000000,
        B11000000,
        B11100000,
        B10100000,
        B10000000,
        B10000000,
        B00000000,
};

const byte neutral[] PROGMEM =
    {
        8,
        7,
        B01110000,
        B11111000,
        B10101000,
        B11111000,
        B10001000,
        B11111000,
        B01110000,
};

//------------------------------------------------------------------------------
//Logo

const byte logo[] PROGMEM = {
    64,
    36,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B10000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000001,
    B10110000,
    B00011000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000001,
    B10111000,
    B00111000,
    B00000000,
    B00111111,
    B10110100,
    B01011101,
    B11011101,
    B11011101,
    B10111100,
    B01111000,
    B00000001,
    B10111111,
    B10100100,
    B01010001,
    B00010101,
    B00010101,
    B10111110,
    B11111011,
    B00000001,
    B10110000,
    B10100100,
    B01010001,
    B00010101,
    B00010101,
    B10111111,
    B11111011,
    B01100001,
    B10110000,
    B00100100,
    B01010001,
    B00010101,
    B00010101,
    B10110111,
    B11011000,
    B01110001,
    B10110000,
    B00100100,
    B01010001,
    B00010101,
    B00010101,
    B10110011,
    B10011011,
    B01110001,
    B10110000,
    B00100100,
    B01010001,
    B00011101,
    B00011101,
    B10110001,
    B00011011,
    B01111001,
    B10111100,
    B00110100,
    B01011001,
    B10010001,
    B10011001,
    B10110000,
    B00011011,
    B01111101,
    B10111000,
    B00010100,
    B01010001,
    B00010001,
    B00010101,
    B10110000,
    B00011010,
    B00101111,
    B10110000,
    B00010100,
    B01010001,
    B00010001,
    B00010101,
    B10110000,
    B00010000,
    B10100111,
    B10110000,
    B00010101,
    B01010001,
    B00010001,
    B00010101,
    B10110000,
    B00000100,
    B10000011,
    B10110000,
    B00010101,
    B01010001,
    B00010001,
    B00010101,
    B10110000,
    B00100011,
    B11010011,
    B10111111,
    B10010101,
    B01010001,
    B00010001,
    B00010101,
    B10110000,
    B00010111,
    B11100001,
    B10111111,
    B10110010,
    B10011101,
    B11010001,
    B11010101,
    B10110000,
    B00001111,
    B11111100,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000101,
    B10110111,
    B10001111,
    B11110001,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11110101,
    B10110000,
    B00111111,
    B11111100,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000101,
    B10110000,
    B00001111,
    B11110000,
    B00000000,
    B00100000,
    B00000000,
    B00000100,
    B10010101,
    B10110000,
    B00010111,
    B11101000,
    B00000000,
    B01000000,
    B10000000,
    B00000010,
    B10100101,
    B10110000,
    B00100011,
    B11000100,
    B00000000,
    B01000001,
    B00000100,
    B00000000,
    B00000101,
    B10110000,
    B00000101,
    B00100000,
    B00000000,
    B10000010,
    B00001000,
    B00100110,
    B10110101,
    B10110000,
    B00000000,
    B10000000,
    B00000001,
    B11000111,
    B00011100,
    B01110000,
    B00000101,
    B10110000,
    B00000001,
    B00000000,
    B00000001,
    B11000111,
    B00011100,
    B01110010,
    B10100101,
    B10110000,
    B00000000,
    B10000000,
    B00000001,
    B11000111,
    B00011100,
    B01110100,
    B10010101,
    B10110000,
    B00000001,
    B00000000,
    B11100000,
    B00000000,
    B00000000,
    B00000000,
    B00000001,
    B10110000,
    B00000000,
    B10000001,
    B11100000,
    B00110010,
    B10000101,
    B11010001,
    B00010101,
    B10110000,
    B00000001,
    B00000001,
    B11100000,
    B00111001,
    B00000101,
    B01010001,
    B00001001,
    B10110000,
    B00000000,
    B10000001,
    B00100000,
    B00110001,
    B00011001,
    B11011101,
    B11001001,
    B10110000,
    B00000001,
    B00000000,
    B00100000,
    B00000000,
    B00000000,
    B00000000,
    B00000001,
    B10110000,
    B00000000,
    B10000000,
    B00100000,
    B00010001,
    B11010101,
    B10011100,
    B11011101,
    B10110000,
    B00000001,
    B00000000,
    B00100000,
    B00111001,
    B01010101,
    B11001000,
    B10001001,
    B10110000,
    B00000000,
    B10000000,
    B01110000,
    B00010001,
    B10011101,
    B10011101,
    B10001001,
    B10110000,
    B00000001,
    B00000000,
    B11111000,
    B00000000,
    B00000000,
    B00000000,
    B00000001,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
};

void playTick()
{
    Piezo.tone(1000, 100);
}

void playOk()
{
    Piezo.tone(2000, 200);
}

void playCancel()
{
    Piezo.tone(1500, 300);
}

//------------------------------------------------------------------------------
// initialize the playing field
void init_board()
{
    for (byte i = 1; i < WIDTH - 1; i++)
    {
        for (byte j = 1; j < HEIGHT - 1; j++)
        {
            board[i][j].is_bomb = false;
            board[i][j].nearby_bombs = 0;
            board[i][j].state = COVERED;
        }
    }
}

//------------------------------------------------------------------------------
// randomly distribute the bombs
void place_bombs()
{
    byte i = 0;
    byte x = 0;
    byte y = 0;

    while (i < BOMB_COUNT)
    {
        x = random(1, WIDTH - 2);
        y = random(1, HEIGHT - 2);

        if (!board[x][y].is_bomb && !(x == cursor.x + 1 && y == cursor.y + 1))
        {
            board[x][y].is_bomb = true;
            i++;
        }
    }
}

//------------------------------------------------------------------------------
// check the eight neighbours for bombs
byte get_bomb_count(byte x, byte y)
{
    byte count = 0;

    if (board[x][y - 1].is_bomb)
        count++;
    if (board[x][y + 1].is_bomb)
        count++;

    if (board[x - 1][y - 1].is_bomb)
        count++;
    if (board[x - 1][y].is_bomb)
        count++;
    if (board[x - 1][y + 1].is_bomb)
        count++;

    if (board[x + 1][y - 1].is_bomb)
        count++;
    if (board[x + 1][y].is_bomb)
        count++;
    if (board[x + 1][y + 1].is_bomb)
        count++;

    return count;
}

//------------------------------------------------------------------------------
// compute the number of adjacent bombs for each field
void compute_bomb_hints()
{
    for (byte i = 1; i < WIDTH - 1; i++)
    {
        for (byte j = 1; j < HEIGHT - 1; j++)
        {
            if (!board[i][j].is_bomb)
            {
                board[i][j].nearby_bombs = get_bomb_count(i, j);
            }
        }
    }
}

//------------------------------------------------------------------------------
// draw the playing field
void draw_board()
{
    // draw happy/sad/nuetral smiley depending on the game state
    if (game_state == LOST)
    {
        sprite->drawChar(
            0x01,
            FIELD_WIDTH,
            FIELD_HEIGHT,
            1);
    }
    else if (a_pressed)
    {
        if (board[cursor.x + 1][cursor.y + 1].state == COVERED)
        {
            sprite->drawBitmap(
                FIELD_WIDTH,
                FIELD_HEIGHT,
                neutral,
                120,
                64,
                TFT_GREEN);
        }
        else
        {
            sprite->drawChar(
                0x02,                
                FIELD_WIDTH,
                FIELD_HEIGHT,
                1);
        }
    }
    else
    {
        sprite->drawChar(
            0x02,            
            FIELD_WIDTH,
            FIELD_HEIGHT,
            1);
    }

    // draw mine fields
    for (byte i = 1; i < WIDTH - 1; i++)
    {

        for (byte j = 1; j < HEIGHT - 1; j++)
        {

            if (board[i][j].state == COVERED)
            {
                sprite->fillRect(
                    offset_x + FIELD_WIDTH * (i - 1) + 1,
                    offset_y + FIELD_HEIGHT * (j - 1) + 1,
                    fontx,
                    fonty,
                    TFT_LIGHTGREY);
            }

            if (board[i][j].state == FLAGGED)
            {

                sprite->drawBitmap(
                    offset_x + FIELD_WIDTH * (i - 1) + 1,
                    offset_y + FIELD_HEIGHT * (j - 1) + 1,
                    flag,
                    fontx,
                    fonty,
                    TFT_RED);
            }

            if (board[i][j].state == UNCOVERED)
            {
                if (board[i][j].is_bomb)
                {
                    sprite->drawChar(
                        0x2A,
                        offset_x + FIELD_WIDTH * (i - 1) + 1,
                        offset_y + FIELD_HEIGHT * (j - 1) + 1,
                        1);
                }
                else
                {
                    sprite->drawChar(
                        text[board[i][j].nearby_bombs],
                        offset_x + FIELD_WIDTH * (i - 1) + 1,
                        offset_y + FIELD_HEIGHT * (j - 1) + 1,
                        1);
                }
            }
        }
    }
    // draw borders
    sprite->drawRect(
        offset_x - 1,
        offset_y - 1,
        LCDWIDTH - 2,
        LCDHEIGHT - 2,
        TFT_CYAN);
}

//------------------------------------------------------------------------------
// draw the cursor
void draw_cursor()
{
    sprite->drawRect(
        (cursor.x - 1) * FIELD_WIDTH + offset_x - 1,
        (cursor.y - 1) * FIELD_HEIGHT + offset_y - 1,
        FIELD_WIDTH + 2,
        FIELD_HEIGHT + 2,
        TFT_BLUE);
}

//------------------------------------------------------------------------------
// uncover all fields with no adjacent bombs and their neighbours
void uncover_harmless_neighbours(byte x, byte y)
{
    if (x > 0 && y > 0 && x < WIDTH - 1 && y < HEIGHT - 1)
    {
        if (board[x][y].state == COVERED)
        {
            if (!board[x][y].is_bomb)
            {
                board[x][y].state = UNCOVERED;
                uncovered_fields++;

                if (board[x][y].nearby_bombs == 0)
                {
                    uncover_harmless_neighbours(x + 1, y + 1);
                    uncover_harmless_neighbours(x + 1, y);
                    uncover_harmless_neighbours(x + 1, y - 1);

                    uncover_harmless_neighbours(x, y + 1);
                    uncover_harmless_neighbours(x, y - 1);

                    uncover_harmless_neighbours(x - 1, y + 1);
                    uncover_harmless_neighbours(x - 1, y);
                    uncover_harmless_neighbours(x - 1, y - 1);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
// process button events
void process_player_input()
{
    // uncover a field
    if (a_pressed)
    {
        NotFirstPress = true;
        if (board[cursor.x + 1][cursor.y + 1].state == COVERED)
        {
            playTick();
        }
    }
    if (!a_pressed && NotFirstPress == true)
    {
        if (first_field)
        {
            first_field = false;
            place_bombs();
            compute_bomb_hints();
        }

        if (board[cursor.x + 1][cursor.y + 1].state == COVERED)
        {

            // game is lost, uncover all bombs
            if (board[cursor.x + 1][cursor.y + 1].is_bomb)
            {

                for (byte x = 1; x < WIDTH - 1; x++)
                {
                    for (byte y = 1; y < HEIGHT - 1; y++)
                    {
                        if (board[x][y].is_bomb)
                        {
                            board[x][y].state = UNCOVERED;
                        }
                    }
                }
                game_state = LOST;
                sprite->drawString((String("You lost. :(")), 0, 120);
                playCancel();
            }
            else
            {
                playOk();

                if (board[cursor.x + 1][cursor.y + 1].nearby_bombs == 0)
                {
                    uncover_harmless_neighbours(cursor.x + 1, cursor.y + 1);
                }
                else
                {
                    board[cursor.x + 1][cursor.y + 1].state = UNCOVERED;
                    uncovered_fields++;
                }

                if (uncovered_fields == COLUMNS * ROWS - BOMB_COUNT)
                {
                    game_state = WON;
                    sprite->drawString(("You won! :)"), 0, 120);
                    playOk();
                }
            }
        }
    }

    // flag and unflag a field
    if (b_pressed)
    {
        if (first_field)
        {
            first_field = false;
            place_bombs();
            compute_bomb_hints();
        }

        if (board[cursor.x + 1][cursor.y + 1].state == COVERED)
        {
            board[cursor.x + 1][cursor.y + 1].state = FLAGGED;
            flag_count++;
            playOk();
        }
        else if (board[cursor.x + 1][cursor.y + 1].state == FLAGGED)
        {
            board[cursor.x + 1][cursor.y + 1].state = COVERED;
            flag_count--;
            playOk();
        }
    }

    // go  back to main menu
    if (c_pressed)
    {
        setup();
    }

    // move cursor
    if (up_pressed)
    {
        if (cursor.y > 0)
            cursor.y -= 1;
        else
            cursor.y = ROWS - 1;
        playTick();
    }

    if (down_pressed)
    {
        if (cursor.y < ROWS - 1)
            cursor.y += 1;
        else
            cursor.y = 0;
        playTick();
    }

    if (left_pressed)
    {
        if (cursor.x > 0)
            cursor.x -= 1;
        else
            cursor.x = COLUMNS - 1;
        playTick();
    }

    if (right_pressed)
    {
        if (cursor.x < COLUMNS - 1)
            cursor.x += 1;
        else
            cursor.x = 0;
        playTick();
    }
}

// Button Callbacks
// check button left
void BTN_LEFT_press()
{
    left_pressed = true;
}
void BTN_LEFT_release()
{
    left_pressed = false;
}

// check button right
void BTN_RIGHT_press()
{
    right_pressed = true;
}
void BTN_RIGHT_release()
{
    right_pressed = false;
}

void BTN_UP_press()
{
    up_pressed = true;
}

void BTN_UP_release()
{
    up_pressed = false;
}

void BTN_DOWN_press()
{
    down_pressed = true;
}

void BTN_DOWN_release()
{
    down_pressed = false;
}

// check button B
void BTN_B_press()
{
    b_pressed = true;
}

void BTN_B_release()
{
    b_pressed = false;
}

// check button A
void BTN_A_press()
{
    a_pressed = true;
}

void BTN_A_release()
{
    a_pressed = false;
}

// check button C
void BTN_C_press()
{
    c_pressed = true;
}

void BTN_C_release()
{
    c_pressed = false;
}

//------------------------------------------------------------------------------
void setup()
{
    Nibble.begin();
    display = Nibble.getDisplay();
    sprite = display->getBaseSprite();

    sprite->clear(TFT_BLACK);
    sprite->setTextColor(TFT_WHITE);
    sprite->setTextFont(2); // Need to set font size to draw characters
    sprite->setCursor(0,0);

    LCDHEIGHT = display->getHeight();
    LCDWIDTH = display->getWidth();

    // left
    Input::getInstance()->setBtnPressCallback(BTN_LEFT, BTN_LEFT_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_LEFT, BTN_LEFT_release);

    // right
    Input::getInstance()->setBtnPressCallback(BTN_RIGHT, BTN_RIGHT_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_RIGHT, BTN_RIGHT_release);

    // up
    Input::getInstance()->setBtnPressCallback(BTN_UP, BTN_UP_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_UP, BTN_UP_release);

    // down
    Input::getInstance()->setBtnPressCallback(BTN_DOWN, BTN_DOWN_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, BTN_DOWN_release);

    // B
    Input::getInstance()->setBtnPressCallback(BTN_B, BTN_B_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_B, BTN_B_release);

    // A
    Input::getInstance()->setBtnPressCallback(BTN_A, BTN_A_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_A, BTN_A_release);

    // C
    Input::getInstance()->setBtnPressCallback(BTN_C, BTN_C_press);
    Input::getInstance()->setBtnReleaseCallback(BTN_C, BTN_C_release);

    game_state = RUNNING;

    init_board();

    cursor.x = 0;
    cursor.y = 0;

    uncovered_fields = 0;
    flag_count = 0;
    first_field = true;

    NotFirstPress = false;

}

//------------------------------------------------------------------------------
// main loop
void loop()
{
    // loop here so that we can capture button inputs
    Input::getInstance()->loop(0);
    sprite->clear(TFT_BLACK);
    
    if (game_state == RUNNING)
    {
        process_player_input();
        draw_board();
        draw_cursor();
        if (c_pressed)
        {
            setup();
        }
    }

    if (game_state == WON)
    {
        draw_board();
        draw_cursor();
        if (c_pressed)
        {
            setup();
        }
    }

    if (game_state == LOST)
    {
        draw_board();
        draw_cursor();
        if (c_pressed)
        {
            setup();
        }
    }
    display->commit();

}