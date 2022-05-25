#include "board.h"
#include "piece.h"
#include "game.h"
#include "iostream"

using namespace std;

vector<Piece> store;
Board gameBoard;
char playerTurn;

Game::Game(char turn, string** board) {
    playerTurn = turn;
    this->gameBoard.setBoard(board);
}

bool Game::isPointValid(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8; 
}

int* Game::findKing(char KingColor){
    int* LocKing = new int[2];
    LocKing[0] = -1;
    LocKing[1] = -1;
    bool KingIsFound = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (this->gameBoard.board[i][j].color == KingColor && this->gameBoard.board[i][j].type == 'K') {
                LocKing[0] = i;
                LocKing[1] = j;
                KingIsFound = true;
                break;
            }
        }
        if (KingIsFound) break;
    }
    return LocKing;
}

bool Game::isCheck(char color){
    int x, y;
    int* locKing = new int[2];
    locKing = findKing(color);
    x = locKing[0];
    y = locKing[1];
    delete[] locKing;

    int dxN[8] = {1, 1, 2, 2, -1, -1, -2, -2};
    int dyN[8] = {2, -2, 1, -1, 2, -2, 1, -1};
    for (int k = 0; k < 8; k++) {
        int nx = x + dxN[k], ny = y + dyN[k];
        if (isPointValid(nx, ny) && gameBoard.board[nx][ny].type == 'N' && gameBoard.board[nx][ny].color != color)
            return true;
    }

    vector<int> dx = {1, 1, -1, -1};
    vector<int> dy = {-1, 1, 1, -1};
    for (int k = 0; k < 4; k++){
        for (int i = 1; i <= 8; i++) {
            int nx = x + dx[k] * i, ny = y + dy[k] * i;
            if (isPointValid(nx, ny) && gameBoard.board[nx][ny].color != color){
                if (i == 1){
                    if (gameBoard.board[nx][ny].type == 'P' && gameBoard.board[nx][ny].color == 'W' && nx > x){
                        return true;
                    }
                    if (gameBoard.board[nx][ny].type == 'P' && gameBoard.board[nx][ny].color == 'B' && nx < x){
                        return true;
                    }
                    if (gameBoard.board[nx][ny].type == 'K'){
                        return true;
                    }
                }
                if (gameBoard.board[nx][ny].type  == 'B' || gameBoard.board[nx][ny].type  == 'Q')
                    return true;
            }

            if (gameBoard.board[nx][ny].color != '-')
                break;
        }
    }

    vector<int> dxR = {0, 0, 1, -1};
    vector<int> dyR = {-1, 1, 0, 0};
    for (int k = 0; k < 4; k++){
        for (int i = 1; i <= 8; i++) {
            int nx = x + dxR[k] * i, ny = y + dyR[k] * i;
            if (isPointValid(nx, ny) && gameBoard.board[nx][ny].color != color){
                if (i == 1)
                    if (gameBoard.board[nx][ny].type == 'K')
                        return true;
                if (gameBoard.board[nx][ny].type == 'R' || gameBoard.board[nx][ny].type == 'Q')
                    return true;
            }
            if (gameBoard.board[nx][ny].color != '-')
                break;
        }

    }
    return false;
}

bool Game::isCheckMate(char color){
    if (!isCheck(color)) return false;
    else {
        for (Piece p : gameBoard.getPieces(color)) {
            vector<pair<int, int> > mv = gameBoard.getValidMoves(p);
            for (int i = 0; i < mv.size(); i++) {
                bool x = move(p, mv[i]);
                if(x && !isCheck(color)) {
                    reverseMove();
                    return false;
                }
                reverseMove();
            }
        }
    }
    return true;
}

void Game::changeTurns(){
    playerTurn = (playerTurn == 'W') ? 'B' : 'W';
}

void Game::reverseMove(){
    for (int i = 0; i < 2; i++) {
        Piece a = store.back();
        store.pop_back();
        gameBoard.board[a.x][a.y] = a;
    }
    changeTurns();
}

bool Game::move(Piece p, pair<int, int> mv){
    store.push_back(*p.clone());
    store.push_back(*gameBoard.board[mv.first][mv.second].clone());
    
    gameBoard.board[p.x][p.y].color = '-'; 
    gameBoard.board[p.x][p.y].type = '-';
    p.x = mv.first, p.y = mv.second;
    gameBoard.board[mv.first][mv.second] = p;

    changeTurns();

    if(isCheck(p.color)){
        return false;
    }
    else{
        return true;
    }
}