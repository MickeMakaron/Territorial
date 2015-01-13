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
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Window/Event.hpp"
////////////////////////////////////////////////

#include "World.hpp"
#include "Team.hpp"
#include "TIME_PER_FRAME.hpp"


World::World(sf::RenderWindow& window)
: mWindow(window)
, mTarget(window)
, mCursorNode(mWindow, mTarget)
, mCamera(mWindow, mTarget)
{
    buildWorld();
}


void World::update()
{
    while (!mCommandQueue.isEmpty())
		mEntitiesGraph.onCommand(mCommandQueue.pop());

    mCamera.update();
    mEntitiesGraph.update(mCommandQueue);
    mCursorNode.update(mCommandQueue);


    mEntitiesGraph.removeWrecks();
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
    sf::Vector2f pos(200, 200);

    Team team1(1 << 0);
    Team team2(1 << 1);

    mTeams.push_back(team1);
    mTeams.push_back(team2);

    mTeams[0].addHostile(team2.getId());

    for(int i = 0; i < 5; i++)
    {
        std::unique_ptr<EntityNode> antHill(new EntityNode(100, pos, mTeams[1], Category::ComputerEntity));
        antHill->setTexture(mTextures.get(1));
        mEntitiesGraph.attachChild(std::move(antHill));

        pos.x += 20;
    }

    pos.y += 50;
    std::unique_ptr<EntityNode> antHill(new EntityNode(100, pos, mTeams[0]));
    antHill->setTexture(mTextures.get(1));
    mEntitiesGraph.attachChild(std::move(antHill));




    mTextures.load(2, "assets/textures/cursor.png");
    mCursorNode.setTexture(mTextures.get(2));

    // place player anthill
    // randomize resource placement
}
