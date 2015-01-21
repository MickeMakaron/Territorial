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
#include "EntityMover.hpp"

EntityNode::Attributes::Attributes(int baseHp, float baseMovementSpeed, int baseAttackDamage, float baseAttackRange)
: baseHp(baseHp)
, hp(baseHp)
, baseMovementSpeed(baseMovementSpeed)
, movementSpeed(baseMovementSpeed)
, baseAttackDamage(baseAttackDamage)
, attackDamage(baseAttackDamage)
, baseAttackRange(baseAttackRange)
, attackRange(baseAttackRange)
{

}

EntityNode::EntityNode(int hp, sf::Vector2f position, Team& team, Category::Type category)
: SceneNode(category)
, mAttributes(hp, 100, 10, 40)
, mHarvestCategory(0)
, mAttackCategory(Category::Entity)
, mHealCategory(0)
, mTeam(team)
, mStateQueue(StateQueue::StatePtr(new EntityState(*this)))
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

const EntityNode::Attributes& EntityNode::getAttributes() const
{
    return mAttributes;
}

void EntityNode::interact(EntityNode* target, bool isAppending)
{
    assert(target && !target->isMarkedForRemoval());

    unsigned int targetTeamId = target->getTeamId();
    unsigned int targetCategory = target->getCategory();

    if(targetCategory & mAttackCategory)
    {
        if(mTeam.isHostile(targetTeamId))
            attack(target, isAppending);
        else if(mTeam.isNeutral(targetTeamId))
        {
            // warnPlayer(target); warn player that such an action will result in hostility
        }
    }
    else if(targetCategory & mHarvestCategory)
        harvest(target, isAppending);
    else if(mTeam.isAllied(targetTeamId))
    {
        if(targetCategory & mHealCategory)
            heal(target, isAppending);
        else
            assist(target, isAppending);
    }
    else
        goTo(target->getPosition(), isAppending);
}


void EntityNode::goTo(sf::Vector2f target, bool isAppending)
{
    if(isAppending)
        mStateQueue.pushState(std::move(StateQueue::StatePtr(new EntityStateMove(*this, target))));
    else
        mStateQueue.setState(std::move(StateQueue::StatePtr(new EntityStateMove(*this, target))));
}

void EntityNode::heal(EntityNode* target, bool isAppending)
{
    //mTarget = target;
    //setState(State::Heal);
}

void EntityNode::attack(EntityNode* target, bool isAppending)
{
    if(isAppending)
        mStateQueue.pushState(std::move(StateQueue::StatePtr(new EntityStateAttack(*this, target))));
    else
        mStateQueue.setState(std::move(StateQueue::StatePtr(new EntityStateAttack(*this, target))));
}

void EntityNode::harvest(EntityNode* target, bool isAppending)
{
    //mTarget = target;
    //setState(State::Harvest);
}

void EntityNode::assist(EntityNode* target, bool isAppending)
{
    //mTarget = target;
    //setState(State::Assist);
}

unsigned int EntityNode::getTeamId() const
{
    return mTeam.getId();
}

void EntityNode::updateCurrent(CommandQueue& commands)
{
    mStateQueue.update();
}

bool EntityNode::isMoving() const
{
    return mStateQueue.getState()->isMoving();
}

void EntityNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

sf::FloatRect EntityNode::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}


bool  EntityNode::isDestroyed() const
{
    return mAttributes.baseHp > 0 && mAttributes.hp <= 0;
}

void EntityNode::damage(int points)
{
    mAttributes.hp -= points;
}

void EntityNode::destroy()
{
    mAttributes.hp = 0;
}

bool EntityNode::isMarkedForRemoval() const
{
    return isDestroyed();
}
