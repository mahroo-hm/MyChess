#include "piece.h"
std::map<string, Texture> pieceTextures;
std::map<string, Texture> pieceTexturesSel;
std::map<string, Texture> pieceTexturesCheck;


Piece::Piece(char color, char type, int x, int y) {
    this->color = color;
    this->type = type;
    this->x = x;
    this->y = y;
    this->loadPieceTextures();
    this->loadPieceTexturesSel();
    //this->loadKingTexturesCheck();
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


void Piece::loadPieceTextures(){
    string id = this->getName();
    if (this->type != '-'){
        if (pieceTextures.find(id) == pieceTextures.end())
            pieceTextures[id].loadFromFile(getPiecePath(id));;
        pieceTextures[id].setSmooth(true);
        this->sprite.setTexture(pieceTextures[id]);
        //sprite.setOrigin(sf::Vector2f(50.f, 30.f));
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
        //sprite.setOrigin(sf::Vector2f(50.f, 30.f));
        float pieceScaleX = (float)(cellSize) / (this->spriteSel.getTexture()->getSize().x);
        float pieceScaleY = (float)(cellSize) / (this->spriteSel.getTexture()->getSize().y);
        this->spriteSel.setScale(pieceScaleX, pieceScaleY);
    }
}

void Piece::loadKingTexturesCheck(){
    string id = this->getName();
    id += "ch";

        if (pieceTexturesCheck.find(id) == pieceTexturesCheck.end())
            pieceTexturesCheck[id].loadFromFile(getPiecePath(id));;
        pieceTexturesCheck[id].setSmooth(true);
        this->sprite.setTexture(pieceTexturesCheck[id]);
        //sprite.setOrigin(sf::Vector2f(50.f, 30.f));
        float pieceScaleX = (float)(cellSize) / (this->sprite.getTexture()->getSize().x);
        float pieceScaleY = (float)(cellSize) / (this->sprite.getTexture()->getSize().y);
        this->sprite.setScale(pieceScaleX, pieceScaleY);
}