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


#include "EntityState.hpp"
#include "EntityNode.hpp"
#include "TIME_PER_FRAME.hpp"

EntityState::EntityState(EntityNode& entity)
: mEntity(entity)
{
    // Do nothing more by default.
}

void EntityState::update()
{
    // Do nothing by default.
}

void EntityState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Do nothing by default.
}

bool EntityState::isDone() const
{
    return true; // State is done by default.
}

void EntityState::initialize()
{
    // Do nothing by default.
}


EntityStateMove::EntityStateMove(EntityNode& entity, sf::Vector2f target)
: EntityState(entity)
, mTarget(target)
{

}

void EntityStateMove::initialize()
{
    mWaypoints = mEntityMover.getPath(mEntity.getPosition(), mTarget);
}

bool EntityStateMove::isDone() const
{
    return mWaypoints.empty();
}

void EntityStateMove::setTarget(sf::Vector2f target)
{
    mTarget = target;
    initialize();
}

void EntityStateMove::update()
{
    if(mWaypoints.empty())
        return;

    EntityMover::Waypoint& wp = mWaypoints.front();

    float step = mEntity.getSpeed() * TIME_PER_FRAME::S;
    while(wp.distance < step)
    {
        mEntity.setPosition(wp.destination);

        step -= wp.distance;
        mWaypoints.pop_front();

        if(mWaypoints.empty())
            return;
        else
            wp = mWaypoints.front();
    }

    mEntity.move(wp.direction * step);
    wp.distance -= step;
}

EntityStateAttack::EntityStateAttack(EntityNode& entity, EntityNode* target)
: EntityState(entity)
, mMoveState(entity, target->getPosition())
, mTarget(target)
{
}

void EntityStateAttack::initialize()
{
    mMoveState.initialize();
}

bool EntityStateAttack::isInAttackRange(sf::Vector2f target) const
{
    sf::Vector2f dVec = target - mEntity.getPosition();
    float dSqrd = dVec.x * dVec.x + dVec.y * dVec.y;
    float attackRange = mEntity.getAttackRange();

    return dSqrd < attackRange * attackRange;
}

void EntityStateAttack::update()
{
    mMoveState.update();

    sf::Vector2f targetPosition = mTarget->getPosition();

    // If target is moving, get new path.
    if(mTarget->isMoving())
        mMoveState.setTarget(targetPosition);

    // Attack if in range.
    if(isInAttackRange(targetPosition))
        mTarget->damage(10);
}

bool EntityStateAttack::isDone() const
{
    return mTarget->isDestroyed();
}
