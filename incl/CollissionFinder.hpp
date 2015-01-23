#ifndef ANTGAME_COLLISSIONFINDER_HPP
#define ANTGAME_COLLISSIONFINDER_HPP

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


////////////////////////////////////////////////
// STD - C++ Standard Library
#include <list>
#include <set>
////////////////////////////////////////////////

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/System/Vector2.hpp"
////////////////////////////////////////////////

class EntityNode;

class CollissionFinder
{
    public:
        struct CollissionData
        {
            EntityNode* lNode;
            EntityNode* rNode;

            float           penetrationDepth;
            sf::Vector2f    unitVector;
        };

        std::list<CollissionData> getCollissions(std::set<std::pair<EntityNode*, EntityNode*>>& nearbyEntities);
};

#endif //ANTGAME_COLLISSIONFINDER_HPP
