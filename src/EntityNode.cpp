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
, mDestination(position)
, mTarget(nullptr)
, mHarvestCategory(0)
, mAttackCategory(Category::Entity)
, mHealCategory(0)
, mTeam(team)
, mDefaultState(new EntityState(*this))
{
    setState(std::move(std::unique_ptr<EntityState>(new EntityState(*this))));

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

void EntityNode::goTo(sf::Vector2f target)
{
    auto waypoints = mEntityMover.getPath(getPosition(), target);
    setState(std::move(std::unique_ptr<EntityStateMove>(new EntityStateMove(*this, target))));

    /*
    mWayPoints.clear();
    mWayPoints.push_back(target);
    mDestination = target;
    mTarget = nullptr;
    */
}

void EntityNode::appendGoTo(sf::Vector2f target)
{
    auto waypoints = mEntityMover.getPath(getPosition(), target);
    pushState(std::move(std::unique_ptr<EntityStateMove>(new EntityStateMove(*this, target))));
}

void EntityNode::interact(EntityNode* target)
{
    assert(target && !target->isMarkedForRemoval());

    unsigned int targetTeamId = target->getTeam();
    unsigned int targetCategory = target->getCategory();

    if(targetCategory & mAttackCategory)
    {
        if(mTeam.isHostile(targetTeamId))
            attack(target);
        else if(mTeam.isNeutral(targetTeamId))
        {
            // warnPlayer(target); warn player that such an action will result in hostility
        }
    }
    else if(targetCategory & mHarvestCategory)
        harvest(target);
    else if(mTeam.isAllied(targetTeamId))
    {
        if(targetCategory & mHealCategory)
            heal(target);
        else
            assist(target);
    }
    else
        goTo(target->getPosition());
}

void EntityNode::heal(EntityNode* target)
{
    mTarget = target;
    //setState(State::Heal);
}

void EntityNode::setState(std::unique_ptr<EntityState> state)
{
    mStateQueue.clear();
    mStateQueue.push_back(std::move(state));
}

void EntityNode::pushState(std::unique_ptr<EntityState> state)
{
    mStateQueue.push_back(std::move(state));
}

void EntityNode::attack(EntityNode* target)
{
    mTarget = target;
    //setState(State::Attack);
}

void EntityNode::harvest(EntityNode* target)
{
    mTarget = target;
    //setState(State::Harvest);
}

void EntityNode::assist(EntityNode* target)
{
    mTarget = target;
    //setState(State::Assist);
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
            mStateQueue.pop_front();
    }
    else
        mDefaultState->update();


/*
    if(mTarget)
    {
        assert(!mTarget->isMarkedForRemoval());

        mWayPoints = mEntityMover.getPath(getPosition(), mTarget->getPosition());

        switch(mState)
        {
            case State::Attack:
                mTarget->damage(10);
                if(mTarget->isMarkedForRemoval())
                {
                    mTarget = nullptr;
                    setState(State::Idle);
                }
                break;
            default:
                break;
        }
    }

    move();

    */
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

bool EntityNode::isMoving() const
{
    return mWayPoints.size() > 0;
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
