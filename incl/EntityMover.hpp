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

#ifndef ANTGAME_ENTITYMOVER_HPP
#define ANTGAME_ENTITYMOVER_HPP

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <list>
////////////////////////////////////////////////

#include "SFML/System/Vector2.hpp"

class EntityMover
{
    public:
        struct Waypoint
        {
            sf::Vector2f    destination;
            sf::Vector2f    direction;
            float           distance;
        };

        std::list<EntityMover::Waypoint> getPath(sf::Vector2f pos, sf::Vector2f destination);
};

#endif // ANTGAME_ENTITYMOVER_HPP
