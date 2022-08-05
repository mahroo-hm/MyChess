#include "board.h"
#include "iostream"

using namespace std;
using namespace sf;


Board::Board (string** board, RenderWindow* _window){
    window = _window;
    this->setBoard(board, window);
}

void Board::setBoard(string** inpBoard, RenderWindow* _window) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++){
            board[i][j] = *(new Piece(inpBoard[i][j][1], inpBoard[i][j][0], i, j));
        }
}

void Board::init(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            board[i][j].rect.setSize(sf::Vector2f(cellSize, cellSize));
            if ((i + j) % 2 == 0)
               board[i][j].rect.setFillColor(cellColor1);
            else
                board[i][j].rect.setFillColor(cellColor2);

            board[i][j].rect.setPosition(getCellPosition(i, j));

            if (board[i][j].type != '-')
                board[i][j].sprite.setPosition(board[i][j].rect.getPosition());
        }
    }
}

Piece Board::getPiece(int x, int y) {
    return board[x][y];
}

bool Board::isPointValid(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8; 
}

vector<pair<int, int> > Board::getValidMoves(Piece p) {
    switch (p.type) {
    case 'K':
        return getKingMoves(p);
        break;
    case 'Q':
        return getQueenMoves(p);
        break;
    case 'B':
        return getBishopMoves(p);
        break;
    case 'R':
        return getRookMoves(p);
        break;
    case 'N':
        return getKnightMoves(p);
        break;
    case 'P':
        return getPawnMoves(p);
        break;
    default:
        pair<int, int> nullpair(0, 0);
        vector<pair<int, int> > nullvect;
        nullvect.push_back(nullpair);
        return nullvect;
        break;
    }
}

vector<Piece> Board::getPieces(char color) {
    vector <Piece> playerPieces;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (board[i][j].color == color)
                playerPieces.push_back(board[i][j]);
        }
    }
    return playerPieces;
}

vector<pair<int, int> > Board::getKingMoves(Piece p) {
    vector<pair<int, int> > mv;
    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++) {
            int nx = p.x + i, ny = p.y + j;
            if ((i == 0 && j == 0) || !isPointValid(nx, ny) || board[nx][ny].color == p.color || board[nx][ny].type == 'K')
                continue;
            bool flag = true;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int nxx = nx + i, nyy = ny + j;
                    if (isPointValid(nxx, nyy) && board[nxx][nyy].type == 'K' && board[nxx][nyy].color != p.color) {
                        flag = false;
                        break;
                    }
                }
                if (!flag)
                    break;
            }
            if (flag)
                mv.push_back({nx, ny});
        }
    }
    return mv;
}

vector<pair<int, int> > Board::getMovesByD(vector<int> dx, vector<int> dy, Piece p) {
    vector<pair<int, int> > mv;
    for (int k = 0; k < dx.size(); k++)
        for (int i = 1; i <= 8; i++) {
            int nx = p.x + dx[k] * i, ny = p.y + dy[k] * i;
            if (!isPointValid(nx, ny)) continue;
            if (board[nx][ny].color == p.color || board[nx][ny].type == 'K'){
                break;
            }
            mv.push_back({nx, ny});
            if (board[nx][ny].color != '-'){
                break;
            }
        }
    return mv;
}

vector<pair<int, int> > Board::getBishopMoves(Piece p) {
    vector<int> dx = {1, 1, -1, -1};
    vector<int> dy = {-1, 1, 1, -1};
    return getMovesByD(dx, dy, p);
}

vector<pair<int, int> > Board::getRookMoves(Piece p) {
    vector<int> dx = {0, 0, 1, -1};
    vector<int> dy = {-1, 1, 0, 0};
    return getMovesByD(dx, dy, p);
}

vector<pair<int, int> > Board::getQueenMoves(Piece p) {
    vector<pair<int, int>> rookMv = getRookMoves(p);
    vector<pair<int, int>> bishopMv = getBishopMoves(p);
    for (auto i : bishopMv){
        rookMv.push_back(i);
    }
    return rookMv;
}

vector<pair<int, int> > Board::getKnightMoves(Piece p) {
    vector<pair<int, int> > mv;
    int dx[8] = {1, 1, 2, 2, -1, -1, -2, -2};
    int dy[8] = {2, -2, 1, -1, 2, -2, 1, -1};
    for (int k = 0; k < 8; k++) {
        int nx = p.x + dx[k], ny = p.y + dy[k];
        if (!isPointValid(nx, ny) || board[nx][ny].type == 'K' || board[nx][ny].color == p.color)
            continue;
        mv.push_back({nx, ny});
    }
    return mv;
}

vector<pair<int, int> > Board::getPawnMoves(Piece p) {
    vector<pair<int, int> > mv;
    for (int j = -1; j <= 1; j++) {
        int nx = p.x + 1 + (p.color == 'W') * -2, ny = p.y + j;
        if (isPointValid(nx, ny) && j == 0 && board[nx][ny].type == '-'){
            if (p.x == 1 && p.color == 'B' && board[nx + 1][ny].type == '-')
                mv.push_back({nx + 1, ny});
            else if (p.x == 6 && p.color == 'W' && board[nx - 1][ny].type == '-')
                mv.push_back({nx - 1, ny});
            mv.push_back({nx, ny});
        }
        if (isPointValid(nx, ny) && j != 0 && board[nx][ny].color != p.color && board[nx][ny].color != '-' && board[nx][ny].type != 'K')
            mv.push_back({nx, ny});
    }
    return mv;
}

