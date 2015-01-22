/****************************************************************
****************************************************************
*
* Territorial - 2D RTS game with dynamic territorial borders.
* Copyright (C) 2015 Mikael Hernvall (mikael.hernvall@gmail.com)
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
****************************************************************
****************************************************************/

#include "AntGame.hpp"

#include "World.hpp"
#include "TIME_PER_FRAME.hpp"

#include <sstream>

AntGame::AntGame(unsigned int sizeX, unsigned int sizeY)
: mWindow(sf::VideoMode(sizeX, sizeY), "Ant game", sf::Style::Titlebar | sf::Style::Close)
, mWorld(mWindow)
{
    mWindow.setMouseCursorVisible(false);
    TIME_PER_FRAME::setAsSeconds(1/60.f);
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

void AntGame::update()
{
    mWorld.update();
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
    sf::Time timePerFrame = sf::Time(sf::seconds(TIME_PER_FRAME::S));
    while(mWindow.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        unsigned int ticks = 0;
        while (timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;
            processInput();

            update();


            if(mStateMachine.isEmpty())
                mWindow.close();

            ticks++;
        }
        render();

        std::ostringstream stream;
        stream << "Ant game | FPS: " << 1 / dt.asSeconds();
        mWindow.setTitle(stream.str());
    }
}
