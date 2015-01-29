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

#ifndef ANTGAME_PATHFINDER_HPP
#define ANTGAME_PATHFINDER_HPP

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <list>
////////////////////////////////////////////////

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/System/Vector2.hpp"
////////////////////////////////////////////////

#include "TerrainCollissionNode.hpp"
#include "Map.hpp"

namespace sf
{
    class RenderTarget;
}

class Pathfinder
{
    public:
        Pathfinder(const Map& map);

        struct Waypoint
        {
            Waypoint(sf::Vector2f from, sf::Vector2f to);
            Waypoint(const TerrainCollissionNode::Path* pPath);
            sf::Vector2f    destination;
            sf::Vector2f    direction;
            float           distance;
        };

        void draw(sf::RenderTarget& target) const;
        std::list<Waypoint> getPath(float diameter, sf::Vector2f pos, sf::Vector2f destination);

    private:
        struct PathNode
        {
            PathNode(float distanceTravelled, float distanceLeft, std::list<PathNode>::iterator iParent, const TerrainCollissionNode::Point* pPoint);
            float f();
            float distanceTravelled;
            float distanceLeft;
            std::list<PathNode>::iterator iParent;
            const TerrainCollissionNode::Point* pPoint;
            const TerrainCollissionNode::Path* pPath;
        };

        typedef const TerrainCollissionNode::Point* PointPtr;
        typedef const TerrainCollissionNode::Path* PathPtr;

        float       getF(PathPtr pPath, sf::Vector2f target) const;
        float       getF(sf::Vector2f start, sf::Vector2f p, sf::Vector2f target) const;
        PointPtr    findSmallestF(sf::Vector2f pos, sf::Vector2f destination, std::list<PointPtr> points) const;
        PathPtr     findSmallestF(float diameter, sf::Vector2f destination, std::list<TerrainCollissionNode::Path*> paths) const;
        bool        lineIntersectsRect(sf::Vector2f p1, sf::Vector2f p2, sf::FloatRect rect) const;
        bool        pathIsObstructed(sf::Vector2f from, sf::Vector2f to) const;

    private:
        const Map& mMap;
};

#endif // ANTGAME_PATHFINDER_HPP
