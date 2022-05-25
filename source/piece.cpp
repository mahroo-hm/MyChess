#include "piece.h"
#include "iostream"

using namespace std;


char color;
char type;
int x, y;

Piece::Piece(char color, char type, int x, int y) {
    this->color = color;
    this->type = type;
    this->x = x;
    this->y= y;
}

string Piece::getName() {
    string s;
    s.push_back(type);
    s.push_back(color);
    return s;
}

Piece* Piece::clone() {
    return new Piece(color, type, x, y);
}

