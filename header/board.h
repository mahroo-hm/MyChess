#ifndef BOARD_H
#define BOARD_H

#include "vector"
#include "piece.h"
#include "essentials.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;



class Board {
public:
    Piece board[8][8];
    RenderWindow* window;
    Board() = default;
    Board (string** board, RenderWindow* _window);

    Piece getPiece(int x, int y);
    void setBoard(string** inpBoard, RenderWindow* _window);
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
    void init();
};

#endif