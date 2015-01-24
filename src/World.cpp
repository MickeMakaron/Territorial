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
, mMap("assets/maps/1.png")
, mCursorNode(mWindow, mTarget)
, mCamera(mWindow, mTarget)
, mEntitiesManager(mMap, mCommandQueue)
{
    buildWorld();
}

void World::update()
{
    mCamera.update();
    mEntitiesManager.update();
    mCursorNode.update(mCommandQueue);


    mCursorNode.removeWrecks();
    mEntitiesManager.removeWrecks();
}


void World::handleEvent(const sf::Event& event)
{
    mCursorNode.handleEvent(event);
    mCamera.handleEvent(event);
}


void World::draw()
{
    //mTarget.draw(mBackground);
    mTarget.draw(mMap);
    mEntitiesManager.draw(mTarget);
    mTarget.draw(mCursorNode);

}

void World::buildWorld()
{
    mTextures.load(1, "assets/textures/anthill_large.png");
    sf::Vector2f pos(200, 200);

    Team team1(1 << 0);
    Team team2(1 << 1);

    mTeams.push_back(team1);
    mTeams.push_back(team2);

    //mTeams[1].addAlly(team1.getId());
    mTeams[0].addHostile(team2.getId());


    std::unique_ptr<EntityNode> antHill(new EntityNode(100, pos, mTeams[0], mEntitiesManager, Category::PlayerEntity));
    antHill->setTexture(mTextures.get(1));
    mEntitiesManager.insertEntity(std::move(antHill));

    pos.y += 50;
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 3; x++)
        {
            std::unique_ptr<EntityNode> antHill(new EntityNode(100, pos, mTeams[1], mEntitiesManager, Category::PlayerEntity));
            antHill->setTexture(mTextures.get(1));
            mEntitiesManager.insertEntity(std::move(antHill));

            pos.x += 50;

        }

        pos.x -= 50 * 3;
        pos.y += 50;
    }



    mTextures.load(2, "assets/textures/cursor.png");
    mCursorNode.setTexture(mTextures.get(2));

    // place player anthill
    // randomize resource placement
}
