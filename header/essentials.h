#ifndef ESSENTIALS_H
#define ESSENTIALS_H

#include "SFML/Graphics.hpp"
#include "piece.h"

using namespace std;
using namespace sf;


static int cellSize = 100;
static int cellOffset = 20;
static Color cellColor = Color::Black;
static CircleShape circ;
static Color defendCol = Color::Magenta;
static Color mateCol = Color::Blue;
static Color validMovesCol = Color::Green;

string standardOutput(int fromi, int fromj, string pieceName, int toi, int toj);
Vector2f getCellPosition(int row,int column);
int getCellIndex(int x);
string** restart();
//string getPiecePath(Piece p, string type);

#endif