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

#include "CollissionHandler.hpp"

#include "Utility.hpp"

CollissionHandler::CollissionHandler(sf::FloatRect area)
: mCollissionFinder(area)
{

}

void CollissionHandler::update()
{
    std::list<CollissionData> collissions = mCollissionFinder.getCollissions();

    for(CollissionFinder::CollissionData& collission : collissions)
    {
        collission.lNode->goTo(collission.lNode->getPosition() + collission.unitVector * 10.f);
        collission.rNode->goTo(collission.rNode->getPosition() - collission.unitVector * 10.f);
    }
}

void CollissionHandler::insert(EntityNode* entity)
{
    mCollissionFinder.insertEntity(entity);
}


