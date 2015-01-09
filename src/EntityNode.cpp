#include "EntityNode.hpp"
#include "CommandQueue.hpp"

////////////////////////////////////////////////
// Super Fast Media Library (SFML)
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////

////////////////////////////////////////////////
// C++ Standard Library
#include <cassert>
////////////////////////////////////////////////

EntityNode::EntityNode(int hp, sf::Vector2f position)
: mHp(hp)
{
}

void EntityNode::setTexture(const sf::Texture& texture)
{
    mSprite.setTexture(texture);
}

void EntityNode::setSprite(sf::Sprite sprite)
{
    mSprite = sprite;
}

void EntityNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    move(10 * dt.asSeconds(), 10 * dt.asSeconds());
}

void EntityNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}


unsigned int EntityNode::getCategory() const
{
    return Category::Entity;
}

sf::FloatRect EntityNode::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

int EntityNode::getHitpoints() const
{
    return mHp;
}

void EntityNode::repair(int points)
{
    assert(points > 0);

    mHp += points;
}

void EntityNode::damage(int points)
{
    mHp -= points;
}

void EntityNode::destroy()
{
    mHp = 0;
}

bool EntityNode::isMarkedForRemoval() const
{
    return mHp <= 0;
}
