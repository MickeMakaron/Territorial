#ifndef ANTGAME_ENTITYNODE_HPP
#define ANTGAME_ENTITYNODE_HPP

////////////////////////////////////////////////
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

// SFML - Simple and Fast Media Library
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Sprite.hpp"
////////////////////////////////////////////////


#include "SceneNode.hpp"

class CommandQueue;
class Team;

class EntityNode : public SceneNode
{
    public:
        EntityNode(int hp, sf::Vector2f position, Team& team, Category::Type category = Category::Entity);

        int	            getHitpoints() const;
        void            repair(int points);
        void            damage(int points);
        void            destroy();

        virtual bool            isMarkedForRemoval() const;
        virtual void            drawCurrent(sf::RenderTarget&, sf::RenderStates) const;
        virtual void            updateCurrent(CommandQueue& commands);
        virtual sf::FloatRect   getBoundingRect() const;

        void setTexture(const sf::Texture& texture);
        void setSprite(sf::Sprite sprite);

        virtual void interact(EntityNode* target);
        virtual void goTo(sf::Vector2f target);

        void attack(EntityNode* target);
        void harvest(EntityNode* target);
        void assist(EntityNode* target);
        void heal(EntityNode* target);

        const unsigned int& getTeam() const;

        bool isMoving() const;

    private:
        void updateOrigin();
        void moveTo(sf::Vector2f target);
        void setState(unsigned int state);

    private:
        enum EntityState
        {
            Idle        = 0,
            Attack      = 1 << 0,
            Assist      = 1 << 1,
            Harvest     = 1 << 2,
            Heal        = 1 << 3,
        };

        int	            mHp;
        sf::Sprite      mSprite;
        float           mSpeed;
        sf::Vector2f    mDestination;
        std::list<sf::Vector2f> mWayPoints;
        EntityNode*     mTarget;

        unsigned int    mHarvestCategory;
        unsigned int    mAttackCategory;
        unsigned int    mHealCategory;
        Team&           mTeam;
        unsigned int    mState;


};

#endif // ANTGAME_ENTITYNODE_HPP
