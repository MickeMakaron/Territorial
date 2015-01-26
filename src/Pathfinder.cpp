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


#include "Pathfinder.hpp"
#include "Utility.hpp"

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////


////////////////////////////////////////////////
// STD - C++ Standard Library
#include <cmath>
#include <cassert>
#include <limits>
////////////////////////////////////////////////

Pathfinder::Pathfinder(const Map& map)
: mMap(map)
{

}

void Pathfinder::draw(sf::RenderTarget& target) const
{
}

bool Pathfinder::lineIntersectsRect(sf::Vector2f p1, sf::Vector2f p2, sf::FloatRect rect) const
{
    float rectRight = rect.left + rect.width;
    float rectBot = rect.top + rect.height;


    if(p1.x < rect.left && p2.x < rect.left)
        return false;
    if(p1.x > rectRight && p2.x > rectRight)
        return false;

    if(p1.y < rect.top && p2.y < rect.top)
        return false;
    if(p1.y > rectBot && p2.y > rectBot)
        return false;


    sf::Vector2f halfRectSize(rect.width / 2, rect.height / 2);
    sf::Vector2f rectCenter(rect.left + halfRectSize.x, rect.top + halfRectSize.y);

    if(     intersects(p1, p2, sf::Vector2f(rect.left, rect.top), sf::Vector2f(rectRight, rectBot))
        ||  intersects(p1, p2, sf::Vector2f(rectRight, rect.top), sf::Vector2f(rect.left, rectBot))
       )
       return true;
    else
        return false;


    float rectRadiusSqrd = (halfRectSize.x + halfRectSize.y) * (halfRectSize.x + halfRectSize.y);
    sf::Vector2f dVec = p1 - rectCenter;
    float dSqrd = dVec.x * dVec.x + dVec.y * dVec.y;
    if(dSqrd < rectRadiusSqrd)
        return true;

    dVec = p2 - rectCenter;
    dSqrd = dVec.x * dVec.x + dVec.y * dVec.y;
    if(dSqrd < rectRadiusSqrd)
        return true;

    return false;
}

float Pathfinder::getF(PathPtr pPath, sf::Vector2f target) const
{
    // F = G + H
    // G = length of path
    // H = distance from path's point to target.
    return pPath->lengthSqrd + lengthSqrd(target - pPath->p->pos);
}

float Pathfinder::getF(sf::Vector2f start, sf::Vector2f p, sf::Vector2f target) const
{
    // F = G + H
    // G = length of path
    // H = distance from path's point to target.
    return lengthSqrd(p - start) + lengthSqrd(p - target);
}

bool Pathfinder::pathIsObstructed(sf::Vector2f from, sf::Vector2f to) const
{
    for(const Map::NodePtr& pNode : mMap.getImpassableTerrain())
        if(lineIntersectsRect(from, to, pNode->getBoundingRect()) && pNode->isLineIntersecting(from, to))
            return true;

    return false;
}


Pathfinder::PointPtr Pathfinder::findSmallestF(sf::Vector2f pos, sf::Vector2f destination, std::list<PointPtr> points) const
{
    assert(!points.empty());

    float minF = std::numeric_limits<float>::max();
    PointPtr pMinPoint = points.front();
    for(PointPtr pPoint : points)
    {
        float f = getF(pos, pPoint->pos, destination);
        if(f < minF)
        {
            minF = f;
            pMinPoint = pPoint;
        }
    }

    return pMinPoint;
}

Pathfinder::PathPtr Pathfinder::findSmallestF(sf::Vector2f destination, std::list<TerrainCollissionNode::Path*> paths) const
{
    assert(!paths.empty());

    float minF = std::numeric_limits<float>::max();
    PathPtr pMinPath = paths.front();
    for(PathPtr pPath : paths)
    {
        float f = getF(pPath, destination);
        if(f < minF)
        {
            minF = f;
            pMinPath = pPath;
        }
    }

    return pMinPath;
}


std::list<Pathfinder::Waypoint> Pathfinder::getPath(sf::Vector2f pos, sf::Vector2f destination)
{
    std::list<Waypoint> wayPoints;

    if(pathIsObstructed(pos, destination))
    {
        PointPtr goal = findSmallestF(pos, destination, mMap.getVisiblePoints(destination));
        PointPtr start = findSmallestF(pos, goal->pos, mMap.getVisiblePoints(pos));

        // Code in here might get used later on.
        {
            /*
            typedef const TerrainCollissionNode::Point* PointPtr;
            std::list<const TerrainCollissionNode*> intersectingNodes;
            std::list<PointPtr> entries;
            std::list<PointPtr> exits;
            for(const Map::NodePtr& pNode : mImpassableTerrain)
            {
                std::pair<PointPtr, PointPtr> entry, exit;
                if(lineIntersectsRect(pos, destination, pNode->getBoundingRect()) && pNode->isLineIntersecting(pos, destination, entry, exit))
                {
                    intersectingNodes.push_back(pNode.get());
                    entries.push_back(entry.first);
                    entries.push_back(entry.second);
                    exits.push_back(exit.first);
                    exits.push_back(exit.second);
                }
            }

            if(intersectingNodes.empty())
            {
                Waypoint wp;
                wp.destination = destination;

                sf::Vector2f dVec = destination - pos;
                float d = sqrt(dVec.x * dVec.x + dVec.y * dVec.y);

                wp.distance = d;
                wp.direction = dVec / d;

                return {wp};
            }


            auto comparatorStart = [pos](PointPtr a, PointPtr b)
            {
                return lengthSqrd(a->pos - pos) < lengthSqrd(b->pos - pos);
            };
            PointPtr start = *std::min_element(entries.begin(), entries.end(), comparatorStart);

            auto comparatorGoal = [destination](PointPtr a, PointPtr b)
            {
                return lengthSqrd(a->pos - destination) < lengthSqrd(b->pos - destination);
            };
            PointPtr goal = *std::max_element(exits.begin(), exits.end(), comparatorGoal);
            */

            /*
            float minSqrd = 99999999999999;
            float dSqrd;
            PointPtr start = nullptr;
            for(const TerrainCollissionNode* pNode : intersectingNodes)
            {
                PointPtr p = pNode->getClosestPoint(pos, &dSqrd);

                if(dSqrd < minSqrd)
                {
                    minSqrd = dSqrd;
                    start = p;
                }
            }

            minSqrd = 999999999999999999;
            PointPtr goal = nullptr;
            for(const TerrainCollissionNode* pNode : intersectingNodes)
            {
                PointPtr p = pNode->getClosestPoint(destination, &dSqrd);

                if(dSqrd < minSqrd)
                {
                    minSqrd = dSqrd;
                    goal = p;
                }
            }
            */
        }

        assert(start && goal);

        auto point = start;
        std::list<PathPtr> paths;
        while(point != goal)
        {
            const std::list<TerrainCollissionNode::Path*>& possiblePaths = point->paths;
            assert(possiblePaths.size() > 0);

            paths.push_back(findSmallestF(goal->pos, possiblePaths));
            point = paths.back()->p;
        }

        Waypoint wp;
        wp.destination = start->pos;
        sf::Vector2f dVec = start->pos - pos;
        wp.distance = sqrtf(lengthSqrd(dVec));
        wp.direction = dVec / wp.distance;

        wayPoints.push_back(wp);

        sf::Vector2f prev = start->pos;
        for(auto path : paths)
        {
            wp.destination = path->p->pos;
            wp.distance = sqrtf(path->lengthSqrd);
            wp.direction = (path->p->pos - prev) / wp.distance;

            prev = path->p->pos;
            wayPoints.push_back(wp);
        }


        wp.destination = destination;
        dVec = destination - goal->pos;
        wp.distance = sqrtf(lengthSqrd(dVec));
        wp.direction = dVec / wp.distance;

        wayPoints.push_back(wp);

    }
    else
    {
        Waypoint wp;
        wp.destination = destination;
        sf::Vector2f dVec = destination - pos;
        wp.distance = sqrtf(lengthSqrd(dVec));
        wp.direction = dVec / wp.distance;
        wayPoints = {wp};
    }

    return wayPoints;
}
