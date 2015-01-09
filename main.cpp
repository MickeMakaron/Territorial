#include "AntGame.hpp"

int main()
{
    unsigned int sizeX, sizeY;
    sizeX = sizeY = 500;

    AntGame game(sizeX, sizeY);
    game.run();
}

