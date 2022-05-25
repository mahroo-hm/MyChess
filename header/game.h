#ifndef GAME_H
#define GAME_H

#include "piece.h"
#include "board.h"
#include "vector"
using namespace std;


class Game{
friend class Board;
private:
    vector<Piece> store;
    bool isPointValid(int x, int y);

public:
    Board gameBoard;
    char playerTurn;
    Game(char turn, string** board);
    int* findKing(char KingColor);
    bool isCheck(char color);
    bool isCheckMate(char color);
    void changeTurns();
    void reverseMove();
    bool move(Piece p, pair<int, int> mv);
};

#endif