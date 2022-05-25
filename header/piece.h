#ifndef PIECE_H
#define PIECE_H

using namespace std;

class Piece {
public:
    char color;
    char type;
    int x, y;

    Piece();
    Piece(char color, char type, int x, int y);
    string getName();
    Piece* clone();
};

#endif