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

#ifndef ANTGAME_ENTITYSTATE_HPP
#define ANTGAME_ENTITYSTATE_HPP

class EntityNode;
#include "EntityMover.hpp"

#include "SFML/Graphics/RenderTarget.hpp"

class EntityState
{
    public:
        EntityState(EntityNode& entity);

        virtual void update();
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual bool isDone() const;
        virtual void initialize();

    protected:
        EntityNode& mEntity;
};


class EntityStateMove : public EntityState
{
    public:
        EntityStateMove(EntityNode& entity, sf::Vector2f target);

        virtual void update();
        virtual bool isDone() const;
        virtual void initialize();

    private:
        std::list<EntityMover::Waypoint>    mWaypoints;
        EntityMover                         mEntityMover;
        sf::Vector2f                        mTarget;
        bool                                mIsInitialized;
};


#endif // ANTGAME_ENTITYSTATE_HPP
