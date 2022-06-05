#include "iostream"
#include "game.h"


using namespace std;

Game::Game(char turn, string** board, RenderWindow* _window) {
    window = _window;
    playerTurn = turn;
    this->window->setFramerateLimit(60);
    this->gameBoard.setBoard(board);
}

void Game::start(){
    this->window->display();
    while (this->window->isOpen()) {
        Event event;

        while (this->window->pollEvent(event)) {
            if (event.type == Event::Closed) {
                this->window->close();
            }
            if (Mouse::isButtonPressed(Mouse::Left)) {
                int xSelected, ySelected;
                Piece p;
                const Vector2i position = Mouse::getPosition(*(this->window));
                int i = getCellIndex(position.y), j = getCellIndex(position.x);
                if (i == -1 || j == -1)
                    return;
                if (this->gameBoard.board[i][j].color == playerTurn){
                    xSelected = i; ySelected = j;
                    p = gameBoard.board[xSelected][ySelected];
                    vector < pair<Piece, pair<int, int>> > critMv;
                    vector<pair<int, int> > validMoves = this->gameBoard.getValidMoves(p);

                    //p.loadPieceImg(p, "sel");

                    for (auto mv : validMoves){
                        Vector2f validDest = getCellPosition(mv.first, mv.second);
                        circ.setPosition(validDest);
                        circ.setFillColor(validMovesCol);
                    }

                    defend(p.color, critMv);
                    if (critMv.size() > 0){
                        for (auto cm : critMv) {
                            if (xSelected == cm.first.x && ySelected == cm.first.y && cm.first.getName() == p.getName()){
                                Vector2f critDestDef = getCellPosition(cm.second.first, cm.second.second);
                                circ.setPosition(critDestDef);
                                circ.setFillColor(defendCol);
                            }
                        }

                    }

                    critMv.clear();
                    mate(p.color, critMv);
                    if (critMv.size() > 0){
                        for (auto cm : critMv) {
                            if (xSelected == cm.first.x && ySelected == cm.first.y && cm.first.getName() == p.getName()){
                                Vector2f critDestMate = getCellPosition(cm.second.first, cm.second.second);
                                circ.setPosition(critDestMate);
                                circ.setFillColor(mateCol);
                            }
                        }
                    }
                }

                else{
                    vector<pair<int, int> > validMoves = gameBoard.getValidMoves(p);
                    bool isValidMove = false;
                    for (auto validm : validMoves){
                        if (make_pair(i, j) == validm){
                            isValidMove = true;
                            break;
                        }
                    }
                    if (isValidMove && move(p, make_pair(i, j))){
                        changeTurns();
                    }
                }
            }
        }
        this->window->clear(sf::Color(150, 150, 150));
        this->updateStatusText();
        this->gameBoard.draw();
        this->window->display();
    }
}

void Game::mouseIsClicked(const Vector2i& position) {  
    int selectedX = -1, selectedY = -1; 
    int i = getCellIndex(position.y), j = getCellIndex(position.x);
    if (i == -1 || j == -1)
        return;
    if (this->gameBoard.board[i][j].color == playerTurn){
        selectedX = i;
        selectedY = j;
        this->window->draw(this->gameBoard.board[selectedX][selectedY].sprite);
    }
    else{
        vector<pair<int, int> > moves = gameBoard.getValidMoves(gameBoard.board[selectedX][selectedY]);
        bool isValidMove = false;
        for (auto mv : moves){
            if (make_pair(i, j) == mv){
                isValidMove = true;
                break;
            }
        }
        if (isValidMove && move(gameBoard.board[selectedX][selectedY], make_pair(i, j))){
            
        }
    }
}

/*void Game::putPieces(int i, int j) {
    XO* xo = new XO(this->curr_user->id);
    xo->sprite.setPosition(this->cells[i][j].rect.getPosition());
    this->cells[i][j].xo = xo;
    this->cells[i][j].cell_status = OCCUPIED;
}

void Game::emptyCellClicked(int i, int j) {
    put_xo_in_cell(i, j);
    if (isGameOver())
        return;
    this->changeTurns();
}*/

void Game::updateStatusText() {
    if (isGameOver())
        status.setString(playerTurn + " Wins!");
    else
        status.setString(playerTurn + " Turn");
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

bool Game::isGameOver() {
    if (isCheckMate(playerTurn)){
        changeTurns();
        return true;
    }
    return false;
}

bool Game::mate(char attacker,vector < pair<Piece, pair<int, int>> > &answer, int depth){
    char defender = (attacker == 'B') ? 'W' : 'B';
    vector <pair<int, int>> validmoves;

    if (depth == 3)
        return this->isCheckMate(defender);

    if (this->isCheckMate(defender)) return false;
    if (this->isCheckMate(attacker)) return false;

    int mvno = 0;

    for (Piece p : this->gameBoard.getPieces(this->playerTurn)) {
        vector<pair<int, int> > mv = this->gameBoard.getValidMoves(p);
        
        for (int i = 0; i < mv.size(); i++) {
            bool x = this->move(p, mv[i]);

            if (x) {
                mvno++;
                bool res = mate(attacker,answer, depth + 1);

                if (res && this->playerTurn != attacker) {
                    if (depth == 0)
                        answer.push_back(make_pair(p, mv[i]));
                    if (depth != 0) {
                        this->reverseMove();
                        return true;
                    }
                }
                if (!res && this->playerTurn == attacker) {
                    this->reverseMove();
                    return false;
                }
            }
            this->reverseMove();
        }
    }

    if (!mvno)
        return false;

    if (this->playerTurn == attacker)
        return false;
    else
        return true;
}

bool Game::defend(char defender,vector < pair<Piece, pair<int, int>> > &answer, int depth){
    char attacker = (defender == 'B') ? 'W' : 'B';
    if (this->isCheckMate(defender)) return true;
    if (this->isCheckMate(attacker)) return false;
    if (depth == 4)
        return false;

    int mvno = 0;

    for (Piece p : this->gameBoard.getPieces(this->playerTurn)) {
        vector<pair<int, int> > mv = this->gameBoard.getValidMoves(p);
        
        for (int i = 0; i < mv.size(); i++) {
            bool x = this->move(p, mv[i]);

            if (x) {
                mvno++;
                bool res = defend(defender, answer,  depth + 1);
                if (depth == 0 && res) {
                    answer.push_back(make_pair(p, mv[i]));
                }
                if (depth != 0) {
                    if (res && this->playerTurn != attacker) {
                        this->reverseMove();
                        return true;
                    }
                    if (!res && this->playerTurn == attacker) {
                        this->reverseMove();
                        return false;
                    }
                }
            }
            this->reverseMove();
        }
    }

    if (!mvno)
        return false;

    if (this->playerTurn == attacker)
        return false;
    else
        return true;
}

vector<string> Game::abreviateMoves(vector<string> moves, Game game) {
    sort(moves.begin(), moves.end());

    vector<string> res;
    for (int i = 0; i < moves.size(); ) {
        int cnt = 0, j = i;
        while (j < moves.size() && moves[j][0] == moves[i][0] && moves[j][1] == moves[i][1])
            j++, cnt++;
        int y = (moves[i][0] - 'a');
        int x = 7 - (moves[i][1] - '1');
        Piece p = this->gameBoard.board[x][y];
        vector<pair<int, int> > mv = this->gameBoard.getValidMoves(p);
        int rcnt = 0;
        for (pair<int, int> m : mv) {
            rcnt += this->move(p, m);
            this->reverseMove();
        }
        if (cnt == rcnt)
            res.push_back(moves[i].substr(0, 4)), i = j;
        else { 
            for (i; i < j; i++)
                res.push_back(moves[i]);
        }
    }
    return res;
}
