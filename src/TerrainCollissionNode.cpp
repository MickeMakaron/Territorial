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

#include <TerrainCollissionNode.hpp>
#include <Utility.hpp>

#include <cassert>

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////


TerrainCollissionNode::TerrainCollissionNode(const std::vector<sf::Vector2f>& points)
{
    setPoints(points);
}

TerrainCollissionNode::TerrainCollissionNode()
{
}




void TerrainCollissionNode::setPoints(const std::vector<sf::Vector2f>& points)
{

    mPoints = points;
    // Make sure the last point is the same as the first.
    if(mPoints.front() != mPoints.back())
        mPoints.push_back(mPoints.front());
    mShape.setPoints(mPoints);

    computeConvexAngles();
}


bool TerrainCollissionNode::lineIntersects(sf::Vector2f p1, sf::Vector2f p2, const std::list<std::unique_ptr<TerrainCollissionNode>>& nodes) const
{
    for(const std::unique_ptr<TerrainCollissionNode>& pNode : nodes)
        if(pNode->isLineIntersecting(p1, p2))
            return true;

    return false;
}

TerrainCollissionNode::Path::Path(Point* from, Point* to, bool isEdge, float passWidth)
: p(to)
, isEdge(isEdge)
, passWidth(passWidth)
{
    sf::Vector2f dVec = to->pos - from->pos;
    lengthSqrd = dVec.x * dVec.x + dVec.y * dVec.y;
    length = sqrtf(lengthSqrd);
    direction = dVec / length;
}

TerrainCollissionNode::Point::Point(sf::Vector2f prev, sf::Vector2f pos, sf::Vector2f next)
: pos(pos)
, prev(prev)
, next(next)
{
    sf::Vector2f bisectorVec = (prev - pos) + (next - pos);
    float l = length(bisectorVec);

    bisector = -bisectorVec / l;
}

// Connect points to other nodes.
void TerrainCollissionNode::connectVisiblePoints(std::unique_ptr<TerrainCollissionNode>& node, std::list<std::unique_ptr<TerrainCollissionNode>>& nodes)
{
    if(node.get() == this)
    {
        connectVisiblePoints(nodes);
        return;
    }

    for(Point& p1 : mConvexPoints)
    {
        for(Point& p2 : node->getConvexAngles())
        {
            if(!convexAngleContains(p1.prev, p1.pos, p1.next, p2.pos) && !convexAngleContains(p2.prev, p2.pos, p2.next, p1.pos))
                if(!lineIntersects(p1.pos, p2.pos, nodes))
                {
                    connectPoints(p1, p2, false);
                    node->connectPoints(p2, p1, false);
                }
        }
    }
/*

    // Make sure we have at least a triangle.
    assert(mConvexPoints.size() > 2);

    // Previous, current and the next point.
    auto iPointsPrev = mPoints.begin();
    auto iPointsCurr = mPoints.begin();
    auto iPointsNext = mPoints.begin();

    for(auto iCurr = mConvexPoints.begin(); iCurr != mConvexPoints.end(); iCurr++)
    {
        iPointsCurr = std::find(iPointsCurr, mPoints.end(), iCurr->pos);

        // If this assert fails, this node has either removed a point from mPoint or
        // added a faulty point to mConvexPoint.
        assert(iPointsCurr != mPoints.end());

        if(iPointsCurr == mPoints.begin())
        {
            iPointsPrev = mPoints.end();
            iPointsPrev--;
        }
        else
            iPointsPrev = iPointsCurr;

        iPointsPrev--;

        iPointsNext = iPointsCurr;
        iPointsNext++;

        assert(iPointsNext != mPoints.end());

        for(Point& p : node->getConvexAngles())
        {
            if(p.pos != iCurr->pos)
            {
                bool isVisible = false;
                bool isEdge = false;

                if((node.get() == this) && (p.pos == *iPointsNext || p.pos == *iPointsPrev))
                {
                    if(!lineIntersects(iCurr->pos, p.pos, nodes))
                    {
                        isVisible = true;
                        isEdge = true;
                    }
                }
                else if(!convexAngleContains(*iPointsPrev, iCurr->pos, *iPointsNext, p.pos) && !convexAngleContains())
                    if(!lineIntersects(iCurr->pos, p.pos, nodes))
                        isVisible = true;

                if(isVisible)
                {
                    connectPoints(*iCurr, p, isEdge);
                    node->connectPoints(p, *iCurr, isEdge);
                }
            }
        }
    }
    */
}


TerrainCollissionNode::Path* TerrainCollissionNode::connectPoints(Point& from, Point& to, bool isEdge)
{
    mPaths.push_back(Path(&from, &to, isEdge));
    from.paths.push_back(&mPaths.back());

    return &mPaths.back();
}

const std::vector<sf::Vector2f>& TerrainCollissionNode::getPoints() const
{
    return mPoints;
}

void TerrainCollissionNode::computePassWidths(std::list<std::unique_ptr<TerrainCollissionNode>>& nodes)
{
    for(Point& p : mConvexPoints)
    {
        sf::Vector2f p1 = p.pos;
        for(Path* pPath : p.paths)
        {
            float shortestDistanceSqrd = 10000.f;


            sf::Vector2f p2 = pPath->p->pos;
            for(auto& pNode : nodes)
            {
                const std::vector<sf::Vector2f>& points = pNode->getPoints();

                auto iCurr = points.begin();
                auto iNext = iCurr;
                iNext++;

                while(iNext != points.end())
                {
                    if(p1 != *iCurr && p1 != *iNext && p2 != *iCurr && p2 != *iNext)
                    {
                        if(pPath->isEdge)
                        {
                            float dCurrSqrd = shortestDistanceSqrd;
                            float dNextSqrd = shortestDistanceSqrd;

                            bool isClockwise = p1 == pPath->p->prev;

                            // If clockwise
                            if(p1 == pPath->p->prev)
                            {
                                if(!isAngleConvex(p1, p2, *iCurr))
                                    dCurrSqrd = minDistanceSqrd(p1, p2, *iCurr);

                                if(!isAngleConvex(p1, p2, *iNext))
                                    dNextSqrd = minDistanceSqrd(p1, p2, *iNext);
                            }
                            else
                            {
                                if(isAngleConvex(p1, p2, *iCurr))
                                    dCurrSqrd = minDistanceSqrd(p1, p2, *iCurr);

                                if(isAngleConvex(p1, p2, *iNext))
                                    dNextSqrd = minDistanceSqrd(p1, p2, *iNext);
                            }

                            float dSqrd = dCurrSqrd < dNextSqrd ? dCurrSqrd : dNextSqrd;
                            if(dSqrd < shortestDistanceSqrd)
                                shortestDistanceSqrd = dSqrd;
                        }
                        else
                        {
                            const std::list<float> distances =
                            {
                                minDistanceSqrd(*iCurr, *iNext, p1), // distance from p1 to the line iCurr->iNext
                                minDistanceSqrd(*iCurr, *iNext, p2), // distance from p2 to the line iCurr->iNext
                                minDistanceSqrd(p1, p2, *iCurr), // distance from iCurr to the line p1->p2
                                minDistanceSqrd(p1, p2, *iNext) // distance from iNext to the line p1->p2
                            };

                            float dSqrd = *std::min_element(distances.begin(), distances.end());
                            if(dSqrd < shortestDistanceSqrd)
                                shortestDistanceSqrd = dSqrd;
                        }

                    }

                    iCurr++;
                    iNext++;
                }
            }


            pPath->passWidth = sqrtf(shortestDistanceSqrd);

        }
    }


}

// Get shortest distance between a line and a point
float TerrainCollissionNode::minDistanceSqrd(sf::Vector2f a, sf::Vector2f b, sf::Vector2f p) const
{
    if(a == sf::Vector2f(300, 100) && b == sf::Vector2f(700, 100))
        int flerp =0 ;

    // If line's length is 0, i.e. its points reside in the same coordinates, the minimum distance is between any of these two points and p.

    if(a == b)
        return lengthSqrd(p - a);



    // l1 = distance between line's points
    // l2 = distance between line's first point and p
    const sf::Vector2f  l1 = b - a;
    const float l1LengthSqrd = lengthSqrd(l1);
    const sf::Vector2f  l2 = p - a;

    const float dotProduct = (l2.x * l1.x + l2.y * l1.y) / l1LengthSqrd;

    // No projection found, p is closest to line's first point.
    if(dotProduct < 0.f)
        return lengthSqrd(p - a);

    // No projection found, p is closest to line's second point.
    if(dotProduct > 1.f)
        return lengthSqrd(p - b);

    // Projection found, return distance between p and projection.
    return lengthSqrd(p - (a + dotProduct * l1));
}


// Connect points on self.
void TerrainCollissionNode::connectVisiblePoints(std::list<std::unique_ptr<TerrainCollissionNode>>& nodes)
{
    for(auto i = mConvexPoints.begin(); i != mConvexPoints.end(); i++)
    {
        auto j = i;
        j++;
        while(j != mConvexPoints.end())
        {
            bool isVisible = false;
            bool isEdge = false;

            sf::Vector2f prev = i->prev;
            sf::Vector2f next = i->next;
            sf::Vector2f curr = i->pos;

            if(j->pos == i->next || j->pos == i->prev)
            {
                if(!lineIntersects(i->pos, j->pos, nodes))
                {
                    isVisible = true;
                    isEdge = true;
                }
            }
            else if(!convexAngleContains(i->prev, i->pos, i->next, j->pos) && !lineIntersects(i->pos, j->pos, nodes))
                isVisible = true;

            if(isVisible)
            {
                connectPoints(*i, *j, isEdge);
                connectPoints(*j, *i, isEdge);
            }

            j++;
        }
    }
    // Code below might get used some day.
    //{
        /*
        for(Point& p1 : mConvexPoints)
        {
            for(const Map::NodePtr& pNode : nodes)
            {
                // Only check other nodes.
                if(pNode.get() != this)
                {
                    const std::list<Point>& points = pNode->getConvexAngles();

                    for(const Point& p2 : points)
                    {
                        if(!lineIntersects(p1.pos, p2.pos, nodes))
                        {
                            mPaths.push_back(Path(&p1, &p2));
                            p1.paths.push_back(&mPaths.back());
                        }
                    }
                }
            }
        }
        */
        /*
        for(Point& convexPoint : mConvexPoints)
        {
            // Sort nodes by shortest distance to the point.
            auto comparator = [convexPoint](const std::unique_ptr<TerrainCollissionNode>& a, const std::unique_ptr<TerrainCollissionNode>& b)
            {
                sf::FloatRect aRect = a->getBoundingRect();
                sf::FloatRect bRect = b->getBoundingRect();

                return lengthSqrd(sf::Vector2f(aRect.left, aRect.top) - convexPoint.pos) < lengthSqrd(sf::Vector2f(bRect.left, bRect.top) - convexPoint.pos);
            };

            std::sort(nodes.begin(), nodes.end(), comparator);


            // Get visible nodes.
            typedef std::pair<double, double> TCNSector;
            std::vector<TCNSector> sectors = {TCNSector(0, 0)};
            std::vector<const TerrainCollissionNode*> visibleNodes;
            std::vector<TCNSector> visibleNodesSectors;
            for(const std::unique_ptr<TerrainCollissionNode>& pNode : nodes)
            {
                TCNSector minMaxAngles = pNode->getMinMaxAngles(convexPoint.pos);
                minMaxAngles.first += PI;
                minMaxAngles.second += PI;

                bool isVisible = false;
                TCNSector& sector = sectors.front();

                if(addSectorIfVisible(sectors, minMaxAngles))
                {
                    visibleNodes.push_back(pNode->get());
                    visibleNodesSectors.push_back(minMaxAngles);
                }


                if(sectors.size() == 1 && (sectors[0].first == PI && sectors[0].second == PI * 3))
                    break;
            }

            for(int i = 0; i < visibleNodes.size(); i++)
            {
                std::list<Point>& points = visibleNodes[i]->getConvexAngles();


                for(int j = 0; i < points.size(); j++Point& p : points)
                {
                    if()
                }
            }
        }
        */
    //}
}


// Angles in a range from 180 to 540 degrees (PI to 3 * PI)
std::pair<double, double> TerrainCollissionNode::getMinMaxAngles(sf::Vector2f pos)
{
    std::pair<double, double> minMaxAngles = std::make_pair(PI * 3, 0);

    double DOUBLE_PI = PI * 2;
    for(const Point& p : mConvexPoints)
    {
        double angle = std::atan2(p.pos.y, p.pos.x) + DOUBLE_PI;
        if(angle < minMaxAngles.first)
            minMaxAngles.first = angle;
        else if(angle > minMaxAngles.second)
            minMaxAngles.second = angle;
    }

    return minMaxAngles;
}

bool TerrainCollissionNode::addSectorIfVisible(std::vector<std::pair<double, double>>& sectors, const std::pair<double, double>& sector)
{
    /*
     * If sector is entirely contained by a sector, return false.
     *
     * If sector is partially obscured by a sector, update that
     * sectors angles, update the sectors and return true.
     *
     * If sector does not intersect with any sector at all, insert
     * sector into sectors, merge sectors and return true.
     */
    for(int i = 0; i < sectors.size(); i++)
    {
        if(sector.second <= sectors[i].second && sector.first >= sectors[i].first)
            return false;

        bool isVisible = false;
        if(sector.first < sectors[i].first && sector.second >= sector.first)
        {
            isVisible = true;
            sectors[i].first = sector.first;
        }
        if(sector.second > sectors[i].second && sector.first <= sector.second)
        {
            isVisible = true;
            sectors[i].second = sector.second;
        }

        if(isVisible)
        {
            mergeSectors(sectors);
            return true;
        }
    }

    sectors.push_back(sector);
    mergeSectors(sectors);
    return true;
}


void TerrainCollissionNode::mergeSectors(std::vector<std::pair<double, double>>& sectors)
{
    bool isOverlapping = false;
    for(auto i = sectors.begin(); i != sectors.end(); i++)
    {
        auto j = i;
        j++;
        while(j != sectors.end())
        {
            if(j->first < i->first && j->second >= i->first)
            {
                i->first = j->first;
                isOverlapping = true;
            }
            if(j->second > i->second && j->first <= i->second)
            {
                i->second = j->second;
                isOverlapping = true;
            }

            if(isOverlapping)
            {
                sectors.erase(j++);
                isOverlapping = false;
            }
            else
                j++;
        }
    }
}


void TerrainCollissionNode::computeConvexAngles()
{
    // Make sure we have at least a triangle.
    assert(mPoints.size() > 3);

    // Previous, current and the next point.
    sf::Vector2f prev, curr, next;

    // Examine the first angle.
    prev = mPoints[mPoints.size() - 2];
    curr = mPoints[0];
    next = mPoints[1];

    mConvexPoints.clear();
    if(isAngleConvex(prev, curr, next))
        mConvexPoints.push_back(Point(prev, curr, next));

    // Now do the rest.
    for(unsigned int i = 1; i < mPoints.size() - 1; i++)
    {
        prev = mPoints[i - 1];
        curr = mPoints[i];
        next = mPoints[i + 1];

        if(isAngleConvex(prev, curr, next))
            mConvexPoints.push_back(Point(prev, curr, next));
    }
}

// Does a convex angle enclose a point?
bool TerrainCollissionNode::convexAngleContains(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f p) const
{
    return isAngleConvex(a, b, p) && isAngleConvex(b, c, p);
}


const TerrainCollissionNode::Point* TerrainCollissionNode::getClosestPoint(sf::Vector2f p, float* minSqrd) const
{
    float min = lengthSqrd(mConvexPoints.front().pos - p);
    const Point* pClosestPoint = &mConvexPoints.front();
    for(const Point& point : mConvexPoints)
    {
        float d = lengthSqrd(point.pos - p);
        if(d < min)
        {
            pClosestPoint = &point;
            min = d;
        }
    }

    if(minSqrd)
        *minSqrd = min;

    return pClosestPoint;
}

bool TerrainCollissionNode::isLineIntersecting(sf::Vector2f a, sf::Vector2f b) const
{
    for(unsigned int i = 0; i < mPoints.size() - 1; i++)
    {
        if(mPoints[i] == a || mPoints[i] == b || mPoints[i + 1] == a || mPoints[i + 1] == b)
        {

        }
        else if(intersects(mPoints[i], mPoints[i + 1], a, b))
            return true;
    }

    return false;
}

/*
bool TerrainCollissionNode::isLineIntersecting(sf::Vector2f a, sf::Vector2f b, std::pair<const Point*, const Point*>& entry, std::pair<const Point*, const Point*>& exit) const
{

    std::list<std::pair<Point, Point>> intersectedEdges;
    std::list<sf::Vector2f> intersections;
    for(unsigned int i = 0; i < mPoints.size() - 1 && intersections.size() < 2; i++)
    {
        sf::Vector2f intersection;
        if(mPoints[i] == a || mPoints[i] == b || mPoints[i + 1] == a || mPoints[i + 1] == b)
        {

        }
        else if(intersects(mPoints[i], mPoints[i + 1], a, b, &intersection))
        {


            intersectedEdges.push_back(std::make_pair(mPoints[i], mPoints[i + 1]));
            intersections.push_back(intersection)
        }

    }

    if(intersections.empty())
        return false;
    else
    {
        assert(intersections.size() == 2);
        if(lengthSqrd(intersections.front() - a) < lengthSqrd(intersections.back() - b))
        {
            entry = intersectedEdges.front();
            exit = intersectedEdges.back();
        }
        else
        {
            entry = intersectedEdges.back();
            exit = intersectedEdges.front();
        }

        return true;
    }


}
*/

void TerrainCollissionNode::getVisiblePoints(sf::Vector2f p, const std::list<std::unique_ptr<TerrainCollissionNode>>& nodes, std::list<const Point*>& visiblePoints) const
{
    for(const Point& p1 : mConvexPoints)
        if(!lineIntersects(p, p1.pos, nodes))
            visiblePoints.push_back(&p1);
}

std::list<TerrainCollissionNode::Point>& TerrainCollissionNode::getConvexAngles()
{
    return mConvexPoints;
}

void TerrainCollissionNode::updateCurrent(CommandQueue&)
{

}

void TerrainCollissionNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape);
}

sf::FloatRect TerrainCollissionNode::getBoundingRect() const
{
    return mShape.getGlobalBounds();
}
