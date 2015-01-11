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

#include "EntityNode.hpp"
#include "Team.hpp"


#include <cassert>

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////

#include "CommandQueue.hpp"
#include "TIME_PER_FRAME.hpp"

EntityNode::EntityNode(int hp, sf::Vector2f position, Team& team, Category::Type category)
: SceneNode(category)
, mHp(hp)
, mSpeed(100)
, mDestination(position)
, mTarget(nullptr)
, mHarvestCategory(0)
, mAttackCategory(0)
, mAssistCategory(Category::Entity)
, mTeam(team)
{
    setPosition(position);
    updateOrigin();
}

void EntityNode::updateOrigin()
{
    sf::FloatRect bounds = mSprite.getGlobalBounds();
    setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void EntityNode::setTexture(const sf::Texture& texture)
{
    mSprite.setTexture(texture);
    updateOrigin();
}

void EntityNode::setSprite(sf::Sprite sprite)
{
    mSprite = sprite;
    updateOrigin();
}

void EntityNode::interact(sf::Vector2f target)
{
    mDestination = target;
    mTarget = nullptr;
}

void EntityNode::interact(EntityNode* target)
{
    assert(target && !target->isMarkedForRemoval());

    if(target->getCategory() & mAttackCategory)
        attack(target);
    else if(target->getCategory() & mHarvestCategory)
        harvest(target);
    else if(target->getCategory() & mAssistCategory)
        assist(target);
    else
        interact(target->getPosition());
}

void EntityNode::attack(EntityNode* target)
{
    mTarget = target;
}

void EntityNode::harvest(EntityNode* target)
{
    mTarget = target;
}

void EntityNode::assist(EntityNode* target)
{
    if(mTeam.isAllied(target->getTeam()))
        mTarget = target;
   // else
       // mDestination = target->getPosition();
}

const unsigned int& EntityNode::getTeam() const
{
    return mTeam.getId();
}

void EntityNode::moveTo(sf::Vector2f target)
{
    sf::Vector2f dVec = target - getPosition();
    float dSqrd = dVec.x * dVec.x + dVec.y * dVec.y;
    float step = mSpeed * TIME_PER_FRAME::S;
    if(dSqrd < step * step)
        setPosition(target);
    else
    {
        float d = sqrtf(dSqrd);
        sf::Vector2f unitVec = dVec / d;
        move(unitVec * step);
    }
}

void EntityNode::updateCurrent(CommandQueue& commands)
{
    if(mTarget)
    {
        assert(!mTarget->isMarkedForRemoval());
        mDestination = mTarget->getPosition();
    }

    if(mDestination != getPosition())
        moveTo(mDestination);

}

void EntityNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
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
