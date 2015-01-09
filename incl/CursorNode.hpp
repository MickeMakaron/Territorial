#ifndef ANTGAME_CURSORNODE_HPP
#define ANTGAME_CURSORNODE_HPP

////////////////////////////////////////////////
// Super Fast Media Library (SFML)
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
////////////////////////////////////////////////

#include "SceneNode.hpp"
#include "CommandQueue.hpp"


class EntityNode;

class CursorNode : public SceneNode
{
    public:
                        CursorNode(sf::RenderTarget& target);

        void setTexture(const sf::Texture& texture);

        bool            hasMoved() const;
        void            handleEvent(const sf::Event& event);
        virtual void    updateCurrent(sf::Time, CommandQueue&);
		virtual void    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual sf::FloatRect getBoundingRect() const;

    private:
        void setVisible();
        void setInvisible();

        void activate(const sf::Event::MouseButtonEvent& event);
        void refreshSelection(CommandQueue& commands);

        void onRightMouseRelease(const sf::Event::MouseButtonEvent& event);

        sf::Vector2f pix2coords(const int& x, const int& y) const;
        sf::Vector2f pix2coords(const sf::Vector2i& pixel) const;

    private:
        sf::Sprite          mSprite;
        sf::IntRect         mTextureRect;
        sf::RenderTarget&   mTarget;
        sf::Vector2f        mLastPos;

        EntityNode*         mSelection;
        EntityNode*         mActivation;

        Command mSelectCommand;
};


#endif // ANTGAME_CURSORNODE_HPP
