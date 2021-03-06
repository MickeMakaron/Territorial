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

#ifndef ANTGAME_ANTGAME_HPP
#define ANTGAME_ANTGAME_HPP

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
////////////////////////////////////////////////

#include "World.hpp"

class StateMachine
{
    public:
        bool isEmpty()
        {
            return false;
        }

        void handleEvent(const sf::Event& event)
        {

        }
};

class AntGame
{

    public:
        AntGame(unsigned int sizeX, unsigned int sizeY);

        void processInput();
        void update();
        void render();
        void run();

    private:
        sf::RenderWindow mWindow; ///< SFML window class.
        StateMachine mStateMachine; ///< State machine for menus and other states.
        World mWorld; ///< World state
};


#endif // ANTGAME_ANTGAME_HPP
