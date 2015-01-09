#include "AntGame.hpp"

#include "World.hpp"


AntGame::AntGame(unsigned int sizeX, unsigned int sizeY)
: mWindow(sf::VideoMode(sizeX, sizeY), "Ant game", sf::Style::Titlebar | sf::Style::Close)
, mWorld(mWindow)
{
    mWindow.setMouseCursorVisible(false);
}

void AntGame::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mStateMachine.handleEvent(event);
        mWorld.handleEvent(event);

        if(event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void AntGame::update(sf::Time dt)
{
    mWorld.update(TIME_PER_FRAME);
}

void AntGame::render()
{
    mWindow.clear();
    mWorld.draw();
    mWindow.display();
}

void AntGame::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(mWindow.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        unsigned int ticks = 0;
        while (timeSinceLastUpdate > TIME_PER_FRAME)
        {
            timeSinceLastUpdate -= TIME_PER_FRAME;
            processInput();

            update(TIME_PER_FRAME);


            if(mStateMachine.isEmpty())
                mWindow.close();

            ticks++;
        }
        render();
    }
}
