#ifndef ANTGAME_SCENENODE_HPP
#define ANTGAME_SCENENODE_HPP


#include "Category.hpp"

////////////////////////////////////////////////
// C++ Standard Library
#include <deque>
#include <list>
#include <set>
#include <memory>
#include <utility>
////////////////////////////////////////////////

////////////////////////////////////////////////
// Super Fast Media Library (SFML)
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

		void            update(sf::Time dt, CommandQueue& commands);

		sf::Vector2f			getWorldPosition() const;
		sf::Transform			getWorldTransform() const;

		void					onCommand(const Command& command, sf::Time dt);
		virtual unsigned int	getCategory() const;

		virtual sf::FloatRect	getBoundingRect() const;
        virtual bool            isMarkedForRemoval() const;


        SceneNode*              getParent() const;

		void					drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
	    virtual void            destroyCurrent();
	    void                    destroyChildren();
	    void                    destroy();

		virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
		virtual void            updateCurrent();
		void					updateChildren(sf::Time dt, CommandQueue& commands);

		virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		void					drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		std::list<Ptr>		    mChildren;
		SceneNode*				mParent;
		Category::Type          mDefaultCategory;
};

#endif // ANTGAME_SCENENODE_HPP
