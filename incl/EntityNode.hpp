#ifndef ANTGAME_ENTITYNODE_HPP
#define ANTGAME_ENTITYNODE_HPP

////////////////////////////////////////////////
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

// Super Fast Media Library (SFML)
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Sprite.hpp"
////////////////////////////////////////////////


#include "SceneNode.hpp"

class CommandQueue;


class EntityNode : public SceneNode
{
    public:
        EntityNode(int hp, sf::Vector2f position = sf::Vector2f(0, 0));

        int	            getHitpoints() const;
        void            repair(int points);
        void            damage(int points);
        void            destroy();
        virtual bool    isMarkedForRemoval() const;
        virtual void    drawCurrent(sf::RenderTarget&, sf::RenderStates) const;
        virtual void    updateCurrent(sf::Time dt, CommandQueue& commands);
        virtual unsigned int getCategory() const;
        virtual sf::FloatRect getBoundingRect() const;

        void setTexture(const sf::Texture& texture);
        void setSprite(sf::Sprite sprite);

    private:
        int	            mHp;
        sf::Sprite      mSprite;
};

#endif // ANTGAME_ENTITYNODE_HPP
