#ifndef MINEFIELD_H
#define MINEFIELD_H

enum FieldState
{
    COVERED,
    REVEALED,
    FLAGGED
};

struct Minefield {
    bool isMine = false;
    int nearByMines = 0;
    FieldState state = COVERED;
};

#endif
