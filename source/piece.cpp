#include "piece.h"
#include "iostream"

Piece::Piece(char color, char type, int x, int y) {
    this->color = color;
    this->type = type;
    this->x = x;
    this->y = y;
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

/*void Piece::loadPieceImg(Piece p, string type){
    if (p.type != '-'){
        texture.loadFromFile(getPiecePath(p, type));
        this->sprite.setTexture(texture);
        float pieceScaleX = (float)cellSize / this->sprite.getTexture()->getSize().x;
        float pieceScaleY = (float)cellSize / this->sprite.getTexture()->getSize().y;
        this->sprite.setScale(pieceScaleX, pieceScaleY);
    }
}*/
