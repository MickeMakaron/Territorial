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


#include "CollissionManager.hpp"

CollissionManager::CollissionManager(sf::FloatRect area)
: mFinder()
, mHandler()
, mQuadtree(area, mQuadtreeNodes)
{
}

void CollissionManager::update()
{
    mQuadtree.update();

    std::list<std::pair<EntityNode*, EntityNode*>> nearbyEntities = mQuadtree.getNearbyEntities();
    mHandler.handleCollissions(mFinder.getCollissions(nearbyEntities));
}

void CollissionManager::insertEntity(EntityNode* entity)
{
    mQuadtree.insertEntity(entity);
}

void CollissionManager::removeWrecks()
{
    mQuadtree.removeWrecks();
}

std::list<const Quadtree*> CollissionManager::getQuadtree() const
{
    std::list<const Quadtree*> quadtree;
    mQuadtree.getQuadtree(quadtree);

    return quadtree;
}
