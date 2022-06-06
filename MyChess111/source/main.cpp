#include "game.h"

using namespace sf;
using namespace std;


int main(){
    
    RenderWindow window(VideoMode(800, 700), "Moon Chess", Style::Titlebar | Style::Close);
    string** firstBoard = restart();
    Game game('W', firstBoard, &window);
    game.start();
    
    return 0;
}