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
EntityNode::EntityNode(int hp, sf::Vector2f position, Team& team, Category::Type category)
: SceneNode(category)
, mHp(hp)
, mSpeed(100)
, mAttackRange(40)
, mHarvestCategory(0)
, mAttackCategory(Category::Entity)
, mHealCategory(0)
, mTeam(team)
, mDefaultState(new EntityState(*this))
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

float EntityNode::getSpeed() const
{
    return mSpeed;
}


void EntityNode::interact(EntityNode* target, bool isAppending)
{
    assert(target && !target->isMarkedForRemoval());

    unsigned int targetTeamId = target->getTeam();
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
        pushState(std::move(std::unique_ptr<EntityStateMove>(new EntityStateMove(*this, target))));
    else
        setState(std::move(std::unique_ptr<EntityStateMove>(new EntityStateMove(*this, target))));
}

void EntityNode::heal(EntityNode* target, bool isAppending)
{
    //mTarget = target;
    //setState(State::Heal);
}

void EntityNode::attack(EntityNode* target, bool isAppending)
{
    if(isAppending)
        pushState(std::move(std::unique_ptr<EntityStateAttack>(new EntityStateAttack(*this, target))));
    else
        setState(std::move(std::unique_ptr<EntityStateAttack>(new EntityStateAttack(*this, target))));
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


void EntityNode::setState(std::unique_ptr<EntityState> state)
{
    mStateQueue.clear();
    state->initialize();
    mStateQueue.push_back(std::move(state));
}

void EntityNode::pushState(std::unique_ptr<EntityState> state)
{
    if(mStateQueue.empty())
        state->initialize();

    mStateQueue.push_back(std::move(state));
}

const unsigned int& EntityNode::getTeam() const
{
    return mTeam.getId();
}


void EntityNode::updateCurrent(CommandQueue& commands)
{
    if(!mStateQueue.empty())
    {
        EntityState* pState = mStateQueue.front().get();
        pState->update();

        if(pState->isDone())
        {
            mStateQueue.pop_front();

            if(!mStateQueue.empty())
                mStateQueue.front()->initialize();
        }

    }
    else
        mDefaultState->update();
}

bool EntityNode::isMoving() const
{
    return !mStateQueue.empty() && mStateQueue.front()->isMoving();
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

bool  EntityNode::isDestroyed() const
{
    return mHp <= 0;
}


float EntityNode::getAttackRange() const
{
    return mAttackRange;
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
