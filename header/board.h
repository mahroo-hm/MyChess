#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "vector"
using namespace std;



class Board {
public:
    Piece board[8][8];
    Board();
    void setBoard(string** inpBoard);
    Board (string** board);
    Piece getPiece(int x, int y);
    bool isPointValid(int x, int y);
    vector<pair<int, int> > getValidMoves(Piece p);
    vector<Piece> getPieces(char color);
    vector<pair<int, int> > getKingMoves(Piece p);
    vector<pair<int, int> > getMovesByD(vector<int> dx, vector<int> dy, Piece p);
    vector<pair<int, int> > getBishopMoves(Piece p);
    vector<pair<int, int> > getRookMoves(Piece p);
    vector<pair<int, int> > getQueenMoves(Piece p);
    vector<pair<int, int> > getKnightMoves(Piece p);
    vector<pair<int, int> > getPawnMoves(Piece p);
};

#endif