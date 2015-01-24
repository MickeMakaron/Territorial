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

#ifndef ANTGAME_ENTITIESMANAGER_HPP
#define ANTGAME_ENTITIESMANAGER_HPP

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <memory>
////////////////////////////////////////////////


////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////


#include "EntityNode.hpp"
#include "CollissionManager.hpp"


class CommandQueue;
class Map;

class EntitiesManager
{
    public:
        EntitiesManager(const Map& map, CommandQueue& commandQueue);

        void update();
        void handleEvent(const sf::Event& event);
        void draw(sf::RenderTarget& target) const;

        void removeWrecks();

        void insertEntity(std::unique_ptr<EntityNode> entity);

        std::list<Pathfinder::Waypoint> getPath(sf::Vector2f a, sf::Vector2f b);

    private:
        CommandQueue&       mCommandQueue;
        SceneNode           mEntitiesGraph;
        CollissionManager   mCollissionManager;
        Pathfinder          mPathfinder;
};

#endif // ANTGAME_ENTITIESMANAGER_HPP
