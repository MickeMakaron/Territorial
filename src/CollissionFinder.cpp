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

#include "CollissionFinder.hpp"
#include "EntityNode.hpp"

std::list<CollissionFinder::CollissionData> CollissionFinder::getCollissions(std::list<std::pair<EntityNode*, EntityNode*>>& nearbyEntities)
{
    std::list<CollissionData> collissions;
    for(auto pair : nearbyEntities)
    {
        float radiusSum = pair.first->getBoundingRect().width / 2 + pair.second->getBoundingRect().width / 2;

        sf::Vector2f dVec = pair.first->getPosition() - pair.second->getPosition();

        float dSqrd = dVec.x * dVec.x + dVec.y * dVec.y;

        if(dSqrd < radiusSum * radiusSum)
        {
            CollissionData collission;
            collission.lNode = pair.first;
            collission.rNode = pair.second;

            float d = sqrtf(dSqrd);
            collission.penetrationDepth = radiusSum - d;
            collission.unitVector = dVec / d;

            collissions.push_back(collission);
        }
    }

    return collissions;
}
