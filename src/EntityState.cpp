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
#include "EntitiesManager.hpp"

EntityState::EntityState(EntityNode& entity, EntitiesManager& entitiesManager)
: mEntity(entity)
, mEntitiesManager(entitiesManager)
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

bool EntityState::isMoving() const
{
    // Not moving by default.
    return false;
}


EntityStateMove::EntityStateMove(EntityNode& entity, EntitiesManager& entitiesManager, sf::Vector2f target)
: EntityState(entity, entitiesManager)
, mTarget(target)
{
}

void EntityStateMove::initialize()
{
    mWaypoints = mEntitiesManager.getPath(mEntity.getPosition(), mTarget);
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

bool EntityStateMove::isMoving() const
{
    return !mWaypoints.empty();
}

void EntityStateMove::update()
{
    if(mWaypoints.empty())
        return;

    Pathfinder::Waypoint& wp = mWaypoints.front();

    float step = mEntity.getAttributes().movementSpeed * TIME_PER_FRAME::S;
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

EntityStateAttack::EntityStateAttack(EntityNode& entity, EntitiesManager& entitiesManger, EntityNode* target)
: EntityStateMove(entity, entitiesManger, target->getPosition())
, mTarget(target)
{
}


bool EntityStateAttack::isInAttackRange(sf::Vector2f target) const
{
    sf::Vector2f dVec = target - mEntity.getPosition();
    float dSqrd = dVec.x * dVec.x + dVec.y * dVec.y;
    float attackRange = mEntity.getAttributes().attackRange;

    return dSqrd < attackRange * attackRange;
}

void EntityStateAttack::update()
{
    EntityStateMove::update();

    sf::Vector2f targetPosition = mTarget->getPosition();

    // If target is moving, get new path.
    if(mTarget->isMoving())
        EntityStateMove::setTarget(targetPosition);

    // Attack if in range.
    if(isInAttackRange(targetPosition))
        mTarget->damage(10);
}

bool EntityStateAttack::isDone() const
{
    return mTarget->isDestroyed();
}
