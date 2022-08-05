#ifndef PIECE_H
#define PIECE_H

#include "essentials.h"
#include "iostream"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Piece {
public:
    char color;
    char type;
    int x, y;
    Sprite sprite;
    Sprite spriteSel;
    Sprite spriteCheck;
    Sprite spriteMate;
    RectangleShape rect;
    Texture texture;

    Piece() = default;
    Piece(char color, char type, int x, int y);
    string getName();
    Piece* clone();
    void setNull();
    void loadPieceTextures();
    void loadPieceTexturesSel();
    void loadKingTexturesCheck();
    void loadKingTexturesMate();


};

#endif