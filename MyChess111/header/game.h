#ifndef GAME_H
#define GAME_H

#include "vector"
#include "board.h"
#include "piece.h"
#include "essentials.h"
#include "iostream"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;


class Game{
friend class Board;
private:
    vector<Piece> store;
    bool isPointValid(int x, int y);

public:
    Board gameBoard;
    char playerTurn;
    Font font;
    Text status;
    Text error;
    Piece selectedPiece;
    bool pieceIsSelected;
    RenderWindow* window;
    vector<CircleShape> validMovesCir;
    vector<CircleShape> critMateMovesCir;
    vector<CircleShape> critDefendMovesCir;
    vector < pair<Piece, pair<int, int>> > critMv;

    
    Game() = default;
    Game(char turn, string** board, RenderWindow* _window);
    void start();
    void draw();
    void selectPieceAndCreateMovesCircle(Piece p);
    void calcAndCreateCritCircle(Piece p);
    int* findKing(char KingColor);
    bool isCheck(char color);
    bool isCheckMate(char color);
    void changeTurns();
    void reverseMove();
    bool move(Piece p, pair<int, int> mv);
    bool isGameOver();
    void putPieceInCell(int i, int j);
    void mouseIsClicked(const sf::Vector2i& position);
    //void emptyCellClicked(int i, int j);
    void updateStatusText();
    //void selectPiece(Piece p);
    bool mate(char attacker, vector < pair<Piece, pair<int, int>> > &answer, int depth = 0);
    bool defend(char defender, vector < pair<Piece, pair<int, int>> > &answer, int depth = 0);
};

#endif