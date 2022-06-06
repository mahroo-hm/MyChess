#include "game.h"

using namespace std;

Game::Game(char turn, string** board, RenderWindow* _window){
    window = _window;
    playerTurn = turn;
    this->window->setFramerateLimit(65);
    this->gameBoard.setBoard(board, window);
    
    font.loadFromFile("resources/fonts/roboto.ttf");
    status.setFont(font);
    status.setCharacterSize(20);
    status.setStyle(Text::Regular);
    status.setFillColor(Color::White);
    status.setPosition(Vector2f(700.f, 30.f));

    error.setFont(font);
    error.setCharacterSize(15);
    error.setStyle(Text::Regular);
    error.setFillColor(Color::White);
    status.setPosition(Vector2f(700.f, 30.f));
}

void Game::draw() {
    this->window->draw(this->status);
    this->window->draw(this->error);

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            this->window->draw(gameBoard.board[i][j].rect);
            if (gameBoard.board[i][j].type != '-'){
                if (selectedPiece.x == gameBoard.board[i][j].x && selectedPiece.y == gameBoard.board[i][j].y && selectedPiece.getName() == gameBoard.board[i][j].getName())
                    this->window->draw(this->gameBoard.board[i][j].spriteSel);
                else{
                    this->window->draw(this->gameBoard.board[i][j].sprite);
                } 
            }
        }
    }
    
    for(int i = 0; i < validMovesCir.size(); i++)
        window->draw(validMovesCir.at(i));
    
    for(int i = 0; i < critDefendMovesCir.size(); i++)
        window->draw(critDefendMovesCir.at(i));
    
    for(int i = 0; i < critMateMovesCir.size(); i++)
        window->draw(critMateMovesCir.at(i));

}

void Game::start(){
    this->gameBoard.init();

    this->window->display();
    while (this->window->isOpen()){
        Event event;

        while (this->window->pollEvent(event)){
            if (event.type == Event::Closed)
                this->window->close();
            if (Mouse::isButtonPressed(Mouse::Left)){
                this->mouseIsClicked(Mouse::getPosition(*(this->window)));
            }    
        }

    this->window->clear(Color(150, 150, 150));
    this->updateStatusText();
    this->draw();
    this->window->display();
    }
}

void Game::selectPieceAndCreateMovesCircle(Piece p){
    selectedPiece = p;
    pieceIsSelected = true;
    validMovesCir.clear();
    gameBoard.board[p.x][p.y].spriteSel.setPosition(gameBoard.board[p.x][p.y].rect.getPosition());

    for (auto mv : this->gameBoard.getValidMoves(p)){
        CircleShape circ;
        Vector2f validDest = getCellPosition(mv.first, mv.second);
        circ.setRadius((float) cellSize / 10);
        circ.setPosition(validDest);
        circ.setFillColor(validMovesCol);
        validMovesCir.push_back(circ);
    }
    
    return;
}

void Game::calcAndCreateCritCircle(Piece p){
    critDefendMovesCir.clear();
    critMateMovesCir.clear();
    critMv.clear();

    
    /*defend(p.color, critMv);
    if (critMv.size() > 0){
        for (auto cm : critMv) {
            if (p.x == cm.first.x && p.y == cm.first.y && cm.first.getName() == p.getName()){
                CircleShape circ;
                Vector2f critDestDef = getCellPosition(cm.second.first, cm.second.second);
                circ.setRadius((float) cellSize / 10);
                circ.setPosition(critDestDef);
                circ.setFillColor(defendCol);
                critDefendMovesCir.push_back(circ);
            }
        }
    }

    critMv.clear();

    mate(p.color, critMv);
    if (critMv.size() > 0){
        for (auto cm : critMv) {
            if (p.x == cm.first.x && p.y == cm.first.y && cm.first.getName() == p.getName()){
                CircleShape circ;
                Vector2f critDestMate = getCellPosition(cm.second.first, cm.second.second);
                circ.setRadius((float) cellSize / 10);
                circ.setPosition(critDestMate);
                circ.setFillColor(mateCol);
                critMateMovesCir.push_back(circ);
            }
        }
    }*/
    
    return;
}

void Game::mouseIsClicked(const Vector2i& position) {
    int i = getCellIndex(position.y), j = getCellIndex(position.x);
    Piece p = gameBoard.board[i][j];

    if (i == -1 || j == -1)
        return;

    if (p.color == playerTurn) {
        selectPieceAndCreateMovesCircle(p);
        //calcAndCreateCritCircle(p);
    }

    else {
        if (pieceIsSelected){
            bool isValidMove = false;
            pair <int, int> mv = make_pair(i, j);
            for (auto validm : gameBoard.getValidMoves(selectedPiece)){
                if (mv == validm){
                    isValidMove = true;
                    break;
                }
            }
            if (isValidMove){

                if (this->move(selectedPiece, mv)){
                    //gameBoard.board[i][j].sprite.setPosition(this->gameBoard.board[mv.first][mv.second].rect.getPosition());
                    //p.sprite.setPosition(this->gameBoard.board[i][j].rect.getPosition());
                    //selectedPiece.sprite.setPosition(gameBoard.board[selectedPiece.x][selectedPiece.y].rect.getPosition());
                    error.setString("bibb" + gameBoard.board[selectedPiece.x][selectedPiece.y].getName() + gameBoard.board[p.x][p.y].getName());
                }
            }
            return;
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
    string msg;
    msg += playerTurn; 
    if (isGameOver())
        status.setString(msg + " Wins!");
    else
        status.setString(msg + "'s Turn");
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
    pieceIsSelected = false;
    //selectedPiece = NULL;
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
    p.sprite.setPosition(this->gameBoard.board[mv.first][mv.second].rect.getPosition());

    store.push_back(*p.clone());
    store.push_back(*gameBoard.board[mv.first][mv.second].clone());
    
    gameBoard.board[p.x][p.y].color = '-'; 
    gameBoard.board[p.x][p.y].type = '-';
    p.x = mv.first, p.y = mv.second;
    gameBoard.board[mv.first][mv.second] = p;
    //gameBoard.board[mv.second][mv.first].sprite.setPosition(this->gameBoard.board[mv.second][mv.first].rect.getPosition());


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

bool Game::mate(char attacker, vector < pair<Piece, pair<int, int>> > &answer, int depth){
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

bool Game::defend(char defender, vector < pair<Piece, pair<int, int>> > &answer, int depth){
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
