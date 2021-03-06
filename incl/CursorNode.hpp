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

#ifndef ANTGAME_CURSORNODE_HPP
#define ANTGAME_CURSORNODE_HPP

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
namespace sf
{
    class RenderWindow;
}
////////////////////////////////////////////////


#include "SceneNode.hpp"
#include "CommandQueue.hpp"
#include "EntitySelector.hpp"
class EntityNode;



class CursorNode : public SceneNode
{
    public:
                        CursorNode(sf::RenderWindow& window, sf::RenderTarget& target);

        void setTexture(const sf::Texture& texture);

        bool            hasMoved() const;
        void            handleEvent(const sf::Event& event);
        virtual void    updateCurrent(CommandQueue&);
		virtual void    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual sf::FloatRect getBoundingRect() const;

        void removeWrecks();

    private:
        void setVisible();
        void setInvisible();

        sf::Vector2f pix2coords(const int& x, const int& y) const;
        sf::Vector2f pix2coords(const sf::Vector2i& pixel) const;

    private:
        sf::Sprite          mSprite;
        sf::IntRect         mTextureRect;
        sf::RenderWindow&   mWindow;
        sf::RenderTarget&   mTarget;

        EntitySelector      mEntitySelector;
};


#endif // ANTGAME_CURSORNODE_HPP
