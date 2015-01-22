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


#include "EntitiesManager.hpp"


#include "CommandQueue.hpp"




EntitiesManager::EntitiesManager(sf::FloatRect mapRect, CommandQueue& commandQueue)
: mCommandQueue(commandQueue)
, mCollissionManager(mapRect)
{

}

void EntitiesManager::insertEntity(std::unique_ptr<EntityNode> entity)
{
    mCollissionManager.insertEntity(entity.get());
    mEntitiesGraph.attachChild(std::move(entity));
}

void EntitiesManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mEntitiesGraph);

    // Quadtree debugging
    sf::RectangleShape shape;
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);

    std::list<const Quadtree*> quadtree = mCollissionManager.getQuadtree();
    for(const Quadtree* quad : quadtree)
    {
        assert(quad != nullptr);

        sf::FloatRect bounds = quad->getBoundingRect();
        shape.setPosition(bounds.left, bounds.top);
        shape.setSize(sf::Vector2f(bounds.width, bounds.height));

        target.draw(shape);
    }
}

void EntitiesManager::removeWrecks()
{
    mCollissionManager.removeWrecks();
    mEntitiesGraph.removeWrecks();
}


void EntitiesManager::update()
{
    while (!mCommandQueue.isEmpty())
		mEntitiesGraph.onCommand(mCommandQueue.pop());

    mEntitiesGraph.update(mCommandQueue);
    mCollissionManager.update();
}
