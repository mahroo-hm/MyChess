#include "iostream"

using namespace std;
#include "vector"

using namespace std;



class Board {
public:
    Piece board[8][8];
    RenderWindow* window;
    Board() = default;
    Board (string** board, RenderWindow* _window);

    Piece getPiece(int x, int y);
    void setBoard(string** inpBoard, RenderWindow* _window);
    bool isPointValid(int x, int y);
    vector<pair<int, int> > getValidMoves(Piece p);
    vector<Piece> getPieces(char color);
    vector<pair<int, int> > getKingMoves(Piece p);
    vector<pair<int, int> > getMovesByD(vector<int> dx, vector<int> dy, Piece p);
    vector<pair<int, int> > getBishopMoves(Piece p);
    vector<pair<int, int> > getRookMoves(Piece p);
    vector<pair<int, int> > getQueenMoves(Piece p);
    vector<pair<int, int> > getKnightMoves(Piece p);
    vector<pair<int, int> > getPawnMoves(Piece p);
    void init();
};


#include "iostream"

using namespace std;

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

#include <vector>


using namespace std;
using namespace sf;


class Game{
friend class Board;
private:
    vector<Piece> store;
    bool isPointValid(int x, int y);

public:
    Board gameBoard;
    char playerTurn;
    char oneInCheck = 'n';
    char oneMate = 'n';
    Font font;
    Text status;
    Text error;
    Piece selectedPiece;
    bool pieceIsSelected;
    RenderWindow* window;
    vector<CircleShape> validMovesCir;
    vector<CircleShape> critMateMovesCir;
    vector<CircleShape> critDefendMovesCir;
    vector < pair<Piece, pair<int, int>> > critMv;

    
    Game() = default;
    Game(char turn, string** board, RenderWindow* _window);
    
    void start();
    void draw();
    bool isGameOver();
    void putPieceInCell(int i, int j);
    void mouseIsClicked(const sf::Vector2i& position);
    //void emptyCellClicked(int i, int j);
    void updateStatusText();
    //void selectPiece(Piece p);
    
    void selectPieceAndCreateMovesCircle(Piece p);
    void calcAndCreateCritCircle(Piece p);

    int* findKing(char KingColor);
    bool isCheck(char color);
    bool isCheckMate(char color);
    void changeTurns();
    void reverseMove();
    bool move(Piece p, pair<int, int> mv);
    
    bool mate(char attacker, int depth = 0);
    bool defend(char defender, int depth = 0);
};

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
    status.setPosition(Vector2f(700.f, 50.f));
}

void Game::draw() {
    
    window->clear();
    this->gameBoard.init();

    this->window->draw(this->status);
    
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            this->window->draw(gameBoard.board[i][j].rect);
        
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (gameBoard.board[i][j].type != '-'){
                if (selectedPiece.type != '-' && pieceIsSelected && selectedPiece.x == gameBoard.board[i][j].x && selectedPiece.y == gameBoard.board[i][j].y && selectedPiece.getName() == gameBoard.board[i][j].getName())
                        this->window->draw(this->gameBoard.board[i][j].spriteSel);
                
                else if (gameBoard.board[i][j].type == 'K' && oneInCheck == playerTurn){
                    if (oneMate == playerTurn)
                        this->window->draw(this->gameBoard.board[i][j].spriteMate);
                    else{
                        cout << gameBoard.board[i][j].getName() << gameBoard.board[i][j].x << endl;
                        this->window->draw(this->gameBoard.board[i][j].spriteCheck);
                    }
                }
                else
                    this->window->draw(this->gameBoard.board[i][j].sprite);
            }   

    if (selectedPiece.type != '-' && pieceIsSelected){
        for(int i = 0; i < validMovesCir.size(); i++)
            window->draw(validMovesCir.at(i));
    
        for(int i = 0; i < critDefendMovesCir.size(); i++)
            window->draw(critDefendMovesCir.at(i));

        for(int i = 0; i < critMateMovesCir.size(); i++)
            window->draw(critMateMovesCir.at(i));
    }

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
    
    defend(p.color);
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

    /*critMv.clear();

    mate(p.color);
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
                    selectedPiece.sprite.setPosition(this->gameBoard.board[mv.first][mv.second].rect.getPosition());
                    
                    int* kingLoc = findKing(playerTurn);
                    if (isCheckMate(playerTurn)){
                        oneInCheck = playerTurn;
                        gameBoard.board[kingLoc[0]][kingLoc[1]].spriteMate.setPosition(this->gameBoard.board[kingLoc[0]][kingLoc[1]].rect.getPosition());
                    }
                    else if (isCheck(playerTurn)){
                        oneMate = playerTurn;
                        gameBoard.board[kingLoc[0]][kingLoc[1]].spriteCheck.setPosition(this->gameBoard.board[kingLoc[0]][kingLoc[1]].rect.getPosition());
                    }
                    else{
                        oneInCheck = 'n';
                        oneMate = 'n';
                    }           
                }   
            }
        }
    }
}

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
    selectedPiece.setNull();
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

bool Game::mate(char attacker, int depth){
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
                bool res = mate(attacker, depth + 1);

                if (res && this->playerTurn != attacker) {
                    if (depth == 0)
                        critMv.push_back(make_pair(p, mv[i]));
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

bool Game::defend(char defender, int depth){
    //cout << "here" << endl;
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
                bool res = defend(defender, depth + 1);
                if (depth == 0 && res) {
                    critMv.push_back(make_pair(p, mv[i]));
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




string standardOutput(int fromi, int fromj, string pieceName, int toi, int toj){
    string output;
    output.push_back((char) fromj + 'a');
    output.push_back((char) (7 - fromi) + '1');
    output += pieceName;
    output.push_back((char) toj + 'a');
    output.push_back((char) (7 - toi) + '1');
    return output;
}

Vector2f getCellPosition(int row,int column) {
    return Vector2f(
        cellOffset + column * (cellSize),
        cellOffset + row * (cellSize));
}

int getCellIndex(int x) {
    x -= cellOffset;
    if (x < 0)
        return -1;
    int index = x / cellSize;
    if (index > 7 || x > index * cellSize + cellSize)
        return -1;
    return index;
}

string** restart(){
    string** firstBoard = new string*[8];
    for (int i = 0; i < 8; i++) firstBoard[i] = new string[8];
    firstBoard[0][0] = "RB";
    firstBoard[0][1] = "NB";
    firstBoard[0][2] = "BB";
    firstBoard[0][3] = "QB";
    firstBoard[0][4] = "KB";
    firstBoard[0][5] = "BB";
    firstBoard[0][6] = "NB";
    firstBoard[0][7] = "RB";
    for (int i = 0; i < 8; i++)
        firstBoard[1][i] = "PB";
    
    firstBoard[7][0] = "RW";
    firstBoard[7][1] = "NW";
    firstBoard[7][2] = "BW";
    firstBoard[7][3] = "QW";
    firstBoard[7][4] = "KW";
    firstBoard[7][5] = "BW";
    firstBoard[7][6] = "NW";
    firstBoard[7][7] = "RW";
    for (int i = 0; i < 8; i++)
        firstBoard[6][i] = "PW";

    return firstBoard;    
}

string getPiecePath(string name) {
    string path = "resources/"; 
    path += name;
    return path + ".png";
}

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



using namespace sf;
using namespace std;


int main(){
    
    RenderWindow window(VideoMode(800, 700), "Moon Chess", Style::Titlebar | Style::Close);
    string** firstBoard = restart();
    Game game('W', firstBoard, &window);
    game.start();
    
    return 0;
}