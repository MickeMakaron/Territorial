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
////////////////////////////////////////////////

#include "SceneNode.hpp"
#include "CommandQueue.hpp"


class EntityNode;

namespace sf
{
    class RenderWindow;
}

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



    private:
        void setVisible();
        void setInvisible();

        void activate();
        void refreshSelection(CommandQueue& commands);
        void updateOutline(const EntityNode* node, sf::RectangleShape& outline);

        void updateSelectionBox(sf::Vector2f mousePos);

        void onRightMouseRelease(const sf::Event::MouseButtonEvent& event);

        sf::Vector2f pix2coords(const int& x, const int& y) const;
        sf::Vector2f pix2coords(const sf::Vector2i& pixel) const;

        void pushSelection(EntityNode* node);
        void pushActivation(EntityNode* node);

    private:
        sf::Sprite          mSprite;
        sf::IntRect         mTextureRect;
        sf::RenderWindow&   mWindow;
        sf::RenderTarget&   mTarget;
        sf::Vector2f        mLastPos;

        struct Highlight
        {
            Highlight(EntityNode* node, sf::RectangleShape outline)
            : node(node)
            , outline(outline)
            {

            }

            EntityNode* node;
            sf::RectangleShape outline;
        };

        std::vector<Highlight>              mSelections;
        std::vector<Highlight>              mActivations;

        sf::Vector2f        mMouseDownPos; ///< Position of last time user pressed mouse button.


        sf::RectangleShape  mSelectionBox;
        bool                mSelectionBoxNeedsUpdate;
        sf::Vector2f        mSelectionBoxStartPos;
        bool                mHasSelectionBox;

        Command mSelectCommand;
        Command mSelectBoxCommand;
};


#endif // ANTGAME_CURSORNODE_HPP
