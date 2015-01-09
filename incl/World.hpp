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

#ifndef ANTGAME_WORLD_HPP
#define ANTGAME_WORLD_HPP

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <list>
#include <memory>
////////////////////////////////////////////////


////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/System/Time.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
////////////////////////////////////////////////

#include "EntityNode.hpp"
#include "ResourceHolder.hpp"
#include "CommandQueue.hpp"
#include "CursorNode.hpp"
#include "CameraNode.hpp"

namespace sf
{
    class RenderWindow;
    class RenderTarget;
}

class World
{
    public:
        World(sf::RenderWindow& window);
        void draw();
        void update(sf::Time dt);
        void handleEvent(const sf::Event& event);


    private:
        void buildWorld();
        void moveView(sf::Time dt);

    private:
        SceneNode mEntitiesGraph;
        sf::Sprite mBackground;
        sf::RenderWindow& mWindow;
        sf::RenderTarget& mTarget;
        sf::View         mView;
        CursorNode mCursorNode;
        CameraNode mCameraNode;
        CommandQueue mCommandQueue;
        ResourceHolder<sf::Texture, int> mTextures;
};

#endif // ANTGAME_HPP
