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
// Super Fast Media Library (SFML)
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Window/Event.hpp"
////////////////////////////////////////////////

#include "World.hpp"



World::World(sf::RenderWindow& window)
: mWindow(window)
, mTarget(window)
, mCursorNode(mTarget)
{
    buildWorld();
}


void World::update(sf::Time dt)
{
    mEntitiesGraph.update(dt, mCommandQueue);
    mCursorNode.update(dt, mCommandQueue);

    while (!mCommandQueue.isEmpty())
		mEntitiesGraph.onCommand(mCommandQueue.pop(), dt);
}

void World::handleEvent(const sf::Event& event)
{
    mCursorNode.handleEvent(event);
}


void World::draw()
{
    mTarget.draw(mBackground);
    mTarget.draw(mEntitiesGraph);
    mTarget.draw(mCursorNode);
}

void World::buildWorld()
{
    sf::Texture bgTexture;
    bgTexture.loadFromFile("assets/textures/grass.png");
    bgTexture.setRepeated(true);
    mTextures.load(0, bgTexture);

    mBackground.setTexture(mTextures.get(0));
    mBackground.setTextureRect(sf::IntRect(0, 0, 500, 500));




    mTextures.load(1, "assets/textures/anthill_large.png");
    std::unique_ptr<EntityNode> antHill(new EntityNode(100));
    antHill->setTexture(mTextures.get(1));
    antHill->setPosition(250, 250);
    mEntitiesGraph.attachChild(std::move(antHill));


    mTextures.load(2, "assets/textures/cursor.png");
    mCursorNode.setTexture(mTextures.get(2));

    // place player anthill
    // randomize resource placement
}
