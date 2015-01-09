#ifndef ANTGAME_ENTITYNODE_HPP
#define ANTGAME_ENTITYNODE_HPP

////////////////////////////////////////////////
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
