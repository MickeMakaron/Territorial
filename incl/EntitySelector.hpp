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

#ifndef ANTGAME_ENTITYSELECTOR_HPP
#define ANTGAME_ENTITYSELECTOR_HPP


#include "Command.hpp"
#include "EntityMover.hpp"

class CommandQueue;
class EntityNode;


#include <list>


#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

namespace sf
{
    class RenderTarget;
}

class EntitySelector
{
    public:
                        EntitySelector();

        void    update(CommandQueue& commands);
		void    draw(sf::RenderTarget& target) const;


        bool isSelecting() const;
        void updateSelection(sf::Vector2f pos);
        void startSelection(sf::Vector2f pos);
        void endSelection(sf::Vector2f pos);
        void interact(sf::Vector2f pos);
        void setPosition(sf::Vector2f pos);

    private:
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

        void activate();
        void refreshSelections(CommandQueue& commands);
        void updateOutline(const EntityNode* node, sf::RectangleShape& outline);

        void pushSelection(EntityNode* node);
        void pushActivation(EntityNode* node);

        void removeWrecks();



    private:

        std::list<Highlight>              mSelections;
        std::list<Highlight>              mActivations;

        sf::Vector2f        mPos; ///< Current position of mouse.

        sf::Vector2f        mSelectionStart; ///< Position of last time user pressed left mouse button.
        sf::Vector2f        mSelectionEnd; ///< Position of last time user released left mouse button.

        sf::RectangleShape  mSelectionBox;
        bool                mHasSelectionBox;
        bool                mIsSelecting;

        Command mSelectCommand;
        Command mSelectBoxCommand;

        EntityMover mEntityMover;
};


#endif // ANTGAME_ENTITYSELECTOR_HPP
