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

#ifndef ANTGAME_COLLISSIONMANAGER_HPP
#define ANTGAME_COLLISSIONMANAGER_HPP

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

#include "Quadtree.hpp"
#include "CollissionFinder.hpp"
#include "CollissionHandler.hpp"

class CollissionManager
{
    public:
        CollissionManager(sf::FloatRect area);

        void    update();
        void    insertEntity(EntityNode* entity);
        void    removeWrecks();

        std::list<const Quadtree*> getQuadtree() const; ///< For Quadtree debugging.

    private:
        CollissionFinder    mFinder;
        CollissionHandler   mHandler;
        Quadtree            mQuadtree;
        std::list<Quadtree::Node> mQuadtreeNodes;
};


#endif // ANTGAME_COLLISSIONMANAGER_HPP
