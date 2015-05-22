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

Pathfinder::PathNode::PathNode(float distanceTravelled, float distanceLeft, std::list<PathNode>::iterator iParent, const TerrainCollissionNode::Point* pPoint)
: distanceTravelled(distanceTravelled)
, distanceLeft(distanceLeft)
, iParent(iParent)
, pPoint(pPoint)
, pPath(nullptr)
{

}

Pathfinder::Waypoint::Waypoint(sf::Vector2f from, sf::Vector2f to)
: destination(to)
{
    sf::Vector2f dVec = to - from;
    distance = length(dVec);
    direction = dVec / distance;
}

Pathfinder::Waypoint::Waypoint(const TerrainCollissionNode::Path* pPath)
: destination(pPath->p->pos)
, distance(pPath->length)
, direction(pPath->direction)
{

}

float Pathfinder::PathNode::f()
{
    return distanceTravelled + distanceLeft;
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

Pathfinder::PathPtr Pathfinder::findSmallestF(float diameter, sf::Vector2f destination, std::list<TerrainCollissionNode::Path*> paths) const
{
    assert(!paths.empty());

    float minF = std::numeric_limits<float>::max();
    PathPtr pMinPath = paths.front();
    for(PathPtr pPath : paths)
    {

        bool fitsPath = pPath->isEdge ? diameter < pPath->passWidth : diameter / 2 < pPath->passWidth;

        if(fitsPath)
        {
            float f = getF(pPath, destination);
            if(f < minF)
            {
                minF = f;
                pMinPath = pPath;
            }
        }
    }

    return pMinPath;
}



std::list<Pathfinder::Waypoint> Pathfinder::getPath(float diameter, sf::Vector2f pos, sf::Vector2f destination)
{
    std::list<Waypoint> wayPoints;

    if(pathIsObstructed(pos, destination))
    {
        std::list<PointPtr> visibleGoalPoints = mMap.getVisiblePoints(destination);
        PointPtr goal = findSmallestF(pos, destination, visibleGoalPoints);

        std::list<PointPtr> visibleStartPoints = mMap.getVisiblePoints(pos);
        PointPtr start = findSmallestF(pos, goal->pos, visibleStartPoints);

        sf::Vector2f goalPos = goal->pos;
        auto sortComparator = [&](PathPtr a, PathPtr b)
        {
            return getF(a, goalPos) < getF(b, goalPos);
        };


        bool hasReachedGoal = false;
        int count = 0;
        std::list<PathPtr> breadCrumbs;
        std::list<PointPtr> visitedPoints;

        std::function<void(PointPtr point)> heuristicDFS;
        heuristicDFS = [&](PointPtr point)
        {
            visitedPoints.push_front(point);

            std::list<TerrainCollissionNode::Path*> paths = point->paths;
            paths.sort(sortComparator);

            sf::Vector2f front = paths.front()->p->pos - goalPos;
            sf::Vector2f back = paths.back()->p->pos - goalPos;

            for(PathPtr path : paths)
            {
                count++;
                if(hasReachedGoal)
                    return;

                breadCrumbs.push_back(path);

                if(path->p->pos == goalPos || count >= 5000)
                {
                    hasReachedGoal = true;
                    return;
                }

                if(std::find(visitedPoints.begin(), visitedPoints.end(), path->p) == visitedPoints.end())
                    heuristicDFS(path->p);
                if(hasReachedGoal)
                    return;

                breadCrumbs.pop_back();
            }


        };

        heuristicDFS(start);



        auto it = breadCrumbs.end();
        it--;
        while(it != breadCrumbs.begin())
        {
            if(std::find(visibleStartPoints.begin(), visibleStartPoints.end(), (*it)->p) != visibleStartPoints.end())
            {
                start = (*it)->p;
                it++;
                breadCrumbs.erase(breadCrumbs.begin(), it);
                break;
            }
            it--;
        }


        for(it = breadCrumbs.begin(); it != breadCrumbs.end(); it++)
        {
            if(std::find(visibleGoalPoints.begin(), visibleGoalPoints.end(), (*it)->p) != visibleGoalPoints.end())
            {
                it++;
                breadCrumbs.erase(it, breadCrumbs.end());
                break;
            }
        }
/*
        for(it = breadCrumbs.begin(); it != breadCrumbs.end(); it++)
        {
            for(PathPtr p : goal->paths)
            {
                if(p->p == (*it)->p)
                {
                    it++;
                    breadCrumbs.erase(it, breadCrumbs.end());
                    break;
                }
            }
        }
*/
/*
        for(it = breadCrumbs.begin(); it != breadCrumbs.end(); it++)
        {
            PointPtr point = (*it)->p;

            auto it2 = breadCrumbs.end();
            it2--;
            while(it2 != it)
            {
                for(PathPtr path : point->paths)
                {

                    if(path->p->pos == (*it2)->p->pos)
                    {
                        PathPtr path1 = *it;
                        PathPtr path2 = *it2;


                        *it = path;

                        while(it2 != it)
                            breadCrumbs.erase(it2--);
                        it2++;

                        break;
                    }
                }

                it2--;
            }


        }
        */

        for(PathPtr path : breadCrumbs)
            wayPoints.push_back(Waypoint(path));

        /*if(!wayPoints.empty())
        wayPoints.pop_front();
        if(!wayPoints.empty())
        wayPoints.pop_back();
*/
        wayPoints.push_front(Waypoint(pos, start->pos));

        if(breadCrumbs.empty())
            wayPoints.push_back(Waypoint(start->pos, destination));
        else
            wayPoints.push_back(Waypoint(breadCrumbs.back()->p->pos, destination));


        //PointPtr goal = findSmallestF(pos, destination, mMap.getVisiblePoints(destination));
        //PointPtr start = findSmallestF(pos, goal->pos, mMap.getVisiblePoints(pos));

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

/**
        std::list<PointPtr> visibleStartPoints = mMap.getVisiblePoints(pos);
        std::list<PointPtr> visibleGoalPoints = mMap.getVisiblePoints(destination);

        assert(!visibleStartPoints.empty());
        assert(!visibleGoalPoints.empty());

        std::list<PathNode> allNodes; // All nodes are stored here.
        std::list<std::list<PathNode>::iterator> visitedNodes; // Iterators to visited nodes in allNodes.
        std::list<std::list<PathNode>::iterator> checkList; // Iterators to nodes to be checked in allNodes.

        TerrainCollissionNode::Point startPoint(sf::Vector2f(0, 0), pos, sf::Vector2f(0, 0));
        std::list<TerrainCollissionNode::Path> startPointPaths;
        auto iParent = allNodes.insert(allNodes.end(), PathNode(0.f, lengthSqrd(pos - destination), allNodes.end(), &startPoint));
        auto iParentCheckList = checkList.insert(checkList.end(), iParent);
        for(PointPtr pPoint : visibleStartPoints)
        {
            startPointPaths.push_back(TerrainCollissionNode::Path(&startPoint, pPoint, false));
            startPoint.paths.push_back(&startPointPaths.back());
            checkList.push_back(allNodes.insert(allNodes.end(), PathNode(lengthSqrd(pPoint->pos - pos), lengthSqrd(pPoint->pos - destination), iParent, pPoint)));
        }

        checkList.erase(iParentCheckList);
        visitedNodes.push_back(iParent);

        auto isLessThan = [](std::list<PathNode>::iterator& a, std::list<PathNode>::iterator& b)
        {
            return a->f() < b->f();
        };
*/

/*
        iParentCheckList = std::min_element(checkList.begin(), checkList.end(), isLessThan);
        iParent = *iParentCheckList;
        checkList.erase(iParentCheckList);
        visitedNodes.push_back(iParent);
*/


        //void findPath(std::list<std::list<PathNode>::iterator>& visitedNodes
/**
        std::function<void()> findNext = [&]()
        {
            sf::Vector2f poss = pos;
            float minF = std::numeric_limits<float>::max();
            auto iNextCheckList = checkList.end();
            auto derp = *iParent;
            for(PathPtr pPath : iParent->pPoint->paths)
            {

                bool isVisited = false;
                for(auto& iVisited : visitedNodes)
                {
                    if(iVisited->pPoint && iVisited->pPoint == pPath->p)
                    {
                        isVisited = true;
                        break;
                    }
                }

                if(!isVisited)
                {
                    // Check if path's point exists in the checklist.
                    auto iChecked = allNodes.end();
                    auto iCheckedCheckList = checkList.begin();
                    while(iCheckedCheckList != checkList.end())
                    {
                        if((*iCheckedCheckList)->pPoint == pPath->p)
                        {
                            iChecked = *iCheckedCheckList;
                            break;
                        }
                        iCheckedCheckList++;
                    }

                    float distanceTravelled = iParent->distanceTravelled + pPath->lengthSqrd;
                    float distanceLeft = lengthSqrd(pPath->p->pos - destination);
                    float f = distanceTravelled + distanceLeft;
                    // If found in checkList.
                    if(iCheckedCheckList != checkList.end())
                    {
                        float checkedF = iChecked->f();
                        // If f value from iParent is smaller than from iChecked's
                        // parent, assign iChecked to iParent and this f.
                        if(f < checkedF)
                        {
                            //iChecked->iParent = iParent;
                            //iChecked->distanceTravelled = distanceTravelled;
                            //iChecked->distanceLeft = distanceLeft;
                            //checkedF = iChecked->f();
                        }

                        if(checkedF <= minF)
                        {
                            minF = checkedF;
                            iNextCheckList = iCheckedCheckList;
                            iParent->pPath = pPath;
                        }

                    }
                    else
                    {
                        auto iInsertedCheckList = checkList.insert(checkList.end(), allNodes.insert(allNodes.end(), PathNode(distanceTravelled, distanceLeft, iParent, pPath->p)));
                        if(f <= minF)
                        {
                            minF = f;
                            iNextCheckList = iInsertedCheckList;
                            iParent->pPath = pPath;
                        }
                    }

                }
            }

            // If no unvisited nodes are found.
            if(iNextCheckList == checkList.end())
            {
                assert(iParent->pPoint != &startPoint);

                // Roll back to previous node.
                iParent = iParent->iParent;
            }
            else
            {
                derp = *iParent;

                assert(iParent->pPath);
                iParent = *iNextCheckList;

                derp = *iParent;

                checkList.erase(iNextCheckList);
                visitedNodes.push_back(iParent);


            }

        };


        auto parentIsGoalPoint = [&]()
        {
            for(PointPtr goalPoint : visibleGoalPoints)
                if(iParent->pPoint == goalPoint)
                    return true;

            return false;
        };

        while(!checkList.empty() && !parentIsGoalPoint())
        {
            findNext();
        }
*/
        /*
        do
        {
            findNext();
        } while(!checkList.empty() && !parentIsGoalPoint());
*/
        // If empty, no path was found.
        /**
        if(!checkList.empty())
        {
            // exit point -> destination
            Waypoint exit(iParent->pPoint->pos, destination);
            wayPoints.push_front(Waypoint(iParent->pPoint->pos, destination));

            Waypoint derp = wayPoints.front();

            // pos -> entry point -> ... -> exit point
            do
            {
                wayPoints.push_front(Waypoint(iParent->iParent->pPath));
                derp = wayPoints.front();
                iParent = iParent->iParent;
            } while(iParent->pPoint != &startPoint);
*/
/*
            if(iParent->iParent->pPoint)
            {
                iParent = iParent->iParent;

                // pos -> entry point -> ... -> exit point
                while(iParent->iParent->pPoint)
                {
                    if(!iParent->pPath)
                        int flurp = 0;
                    wayPoints.push_front(Waypoint(iParent->pPath));
                    iParent = iParent->iParent;
                }

                // Entry point's path
                wayPoints.push_front(Waypoint(iParent->pPath));
            }
            */

            // pos -> entry point
            //wayPoints.push_front(Waypoint(pos, iParent->pPoint->pos));

/**        }

        for(Waypoint p : wayPoints)
        {
            int flerp = 0;
        }*/
    }
    else
        wayPoints.push_back(Waypoint(pos, destination));

    return wayPoints;



/*

        assert(start && goal);
        auto point = start;
        std::list<PathPtr> paths;
        while(point != goal)
        {
            const std::list<TerrainCollissionNode::Path*>& possiblePaths = point->paths;
            assert(possiblePaths.size() > 0);


            paths.push_back(findSmallestF(diameter, goal->pos, possiblePaths));
            point = paths.back()->p;
        }
*/

/*
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
*/
}
