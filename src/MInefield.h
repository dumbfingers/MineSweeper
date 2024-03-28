enum FieldState
{
    COVERED,
    REVEALED,
    FLAGGED
};

typedef struct Minefield {
    bool isMine;
    int nearByMines;
    FieldState state;
};