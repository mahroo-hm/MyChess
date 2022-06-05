#ifndef GAME_H
#define GAME_H

#include "vector"
#include "board.h"
#include "piece.h"
#include "critical.h"
#include "essentials.h"
#include <SFML/Graphics.hpp>
using namespace std;


class Game{
friend class Board;
private:
    vector<Piece> store;
    bool isPointValid(int x, int y);

public:
    Board gameBoard;
    char playerTurn;
    bool gameOver;
    Font font;
    Text status;
    RenderWindow* window;
    
    Game() = default;
    Game(char turn, string** board, RenderWindow* window);
    void start();
    int* findKing(char KingColor);
    bool isCheck(char color);
    bool isCheckMate(char color);
    void changeTurns();
    void reverseMove();
    bool move(Piece p, pair<int, int> mv);
    bool isGameOver();
    void mouseIsClicked(const sf::Vector2i& position);
    void emptyCellClicked(int i, int j);
    void updateStatusText();
    void selectPiece(Piece p);
    bool mate(char attacker, vector < pair<Piece, pair<int, int>> > &answer, int depth = 0);
    bool defend(char defender, vector < pair<Piece, pair<int, int>> > &answer, int depth = 0);
    vector<string> abreviateMoves(vector<string> moves, Game game);
};

#endif