#include "piece.h"
std::map<string, Texture> pieceTextures;
std::map<string, Texture> pieceTexturesSel;
std::map<string, Texture> pieceTexturesCheck;
std::map<string, Texture> pieceTexturesMate;



Piece::Piece(char color, char type, int x, int y) {
    this->color = color;
    this->type = type;
    this->x = x;
    this->y = y;
    this->loadPieceTextures();
    this->loadPieceTexturesSel();
    this->loadKingTexturesCheck();
    this->loadKingTexturesMate();

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

void Piece::setNull(){
    this->color = '-';
    this->type = '-';
}

void Piece::loadPieceTextures(){
    string id = this->getName();
    if (this->type != '-'){
        if (pieceTextures.find(id) == pieceTextures.end())
            pieceTextures[id].loadFromFile(getPiecePath(id));;
        pieceTextures[id].setSmooth(true);
        this->sprite.setTexture(pieceTextures[id]);
        float pieceScaleX = (float)(cellSize) / (this->sprite.getTexture()->getSize().x);
        float pieceScaleY = (float)(cellSize) / (this->sprite.getTexture()->getSize().y);
        this->sprite.setScale(pieceScaleX, pieceScaleY);
    }
}

void Piece::loadPieceTexturesSel(){
    string id = this->getName();
    id += "sel";
    if (this->type != '-'){
        if (pieceTexturesSel.find(id) == pieceTexturesSel.end())
            pieceTexturesSel[id].loadFromFile(getPiecePath(id));;
        pieceTexturesSel[id].setSmooth(true);
        this->spriteSel.setTexture(pieceTexturesSel[id]);
        float pieceScaleX = (float)(cellSize) / (this->spriteSel.getTexture()->getSize().x);
        float pieceScaleY = (float)(cellSize) / (this->spriteSel.getTexture()->getSize().y);
        this->spriteSel.setScale(pieceScaleX, pieceScaleY);
    }
}

void Piece::loadKingTexturesCheck(){
    string id = this->getName();
    id += "C";
    if (this->type != '-'){
        if (pieceTexturesCheck.find(id) == pieceTexturesCheck.end())
            pieceTexturesCheck[id].loadFromFile(getPiecePath(id));;
        pieceTexturesCheck[id].setSmooth(true);
        this->spriteCheck.setTexture(pieceTexturesCheck[id]);
        float pieceScaleX = (float)(cellSize) / (this->spriteCheck.getTexture()->getSize().x);
        float pieceScaleY = (float)(cellSize) / (this->spriteCheck.getTexture()->getSize().y);
        this->spriteCheck.setScale(pieceScaleX, pieceScaleY);
    }
}

void Piece::loadKingTexturesMate(){
    string id = this->getName();
    id += "M";
    if (this->type != '-'){
        if (pieceTexturesMate.find(id) == pieceTexturesCheck.end())
            pieceTexturesMate[id].loadFromFile(getPiecePath(id));;
        pieceTexturesMate[id].setSmooth(true);
        this->spriteMate.setTexture(pieceTexturesMate[id]);
        float pieceScaleX = (float)(cellSize) / (this->spriteMate.getTexture()->getSize().x);
        float pieceScaleY = (float)(cellSize) / (this->spriteMate.getTexture()->getSize().y);
        this->spriteMate.setScale(pieceScaleX, pieceScaleY);
    }
}


