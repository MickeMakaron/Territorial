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

#ifndef ANTGAME_SCENENODE_HPP
#define ANTGAME_SCENENODE_HPP


#include "Category.hpp"

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <deque>
#include <list>
#include <set>
#include <memory>
#include <utility>
////////////////////////////////////////////////

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/System/NonCopyable.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Shape.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// Forward declaration
struct Command;
class CommandQueue;
////////////////////////////////////////////////



class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
	public:
		typedef std::unique_ptr<SceneNode> Ptr;
		typedef std::pair<SceneNode*, SceneNode*> Pair;


	public:
		explicit				SceneNode(Category::Type category = Category::None);

		void					attachChild(Ptr child);
		Ptr						detachChild(const SceneNode& node);

        void                    eraseChildren();

		void            update(CommandQueue& commands);

		sf::Vector2f			getWorldPosition() const;
		sf::Transform			getWorldTransform() const;

		void					onCommand(const Command& command);
		virtual unsigned int	getCategory() const;

		virtual sf::FloatRect	getBoundingRect() const;
        virtual bool            isMarkedForRemoval() const;


        SceneNode*              getParent() const;

		void					drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
	    virtual void            destroyCurrent();
	    void                    destroyChildren();
	    void                    destroy();

		virtual void			updateCurrent(CommandQueue& commands);
		virtual void            updateCurrent();
		void					updateChildren(CommandQueue& commands);

		virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		void					drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		std::list<Ptr>		    mChildren;
		SceneNode*				mParent;
		Category::Type          mDefaultCategory;
};


#endif // ANTGAME_SCENENODE_HPP
