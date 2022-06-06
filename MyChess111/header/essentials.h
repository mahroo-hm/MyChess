#ifndef ESSENTIALS_H
#define ESSENTIALS_H

#include "SFML/Graphics.hpp"
#include "iostream"

using namespace std;
using namespace sf;

static int cellSize = 80;
static int cellOffset = 20;
static Color cellColor1 = Color(163, 151, 150);
static Color cellColor2 = Color(156, 36, 36);
static Color defendCol = Color::Magenta;
static Color mateCol = Color::Blue;
static Color validMovesCol = Color::Green;

string standardOutput(int fromi, int fromj, string pieceName, int toi, int toj);
Vector2f getCellPosition(int row,int column);
int getCellIndex(int x);
string** restart();
string getPiecePath(string name);

#endif