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

    initializeConvexPoints(computeConvexAngles());
    computeVisiblePoints();
}

void TerrainCollissionNode::initializeConvexPoints(std::vector<sf::Vector2f> convexAngles)
{
    Point p;
    mConvexPoints.clear();

    // Initialize position.
    for(sf::Vector2f point : convexAngles)
    {
        p.pos = point;
        mConvexPoints.push_back(p);
    }
}

bool TerrainCollissionNode::lineIntersects(sf::Vector2f p1, sf::Vector2f p2, std::list<std::unique_ptr<TerrainCollissionNode>>& nodes) const
{
    for(const std::unique_ptr<TerrainCollissionNode>& pNode : nodes)
        if(pNode->isLineIntersecting(p1, p2))
            return true;

    return false;
}

TerrainCollissionNode::Path::Path(const Point* from, const Point* to, float passWidth)
: p(to)
, passWidth(passWidth)
{
    sf::Vector2f dVec = from->pos - to->pos;
    lengthSqrd = dVec.x * dVec.x + dVec.y * dVec.y;
}

// Compute visible points on self.
void TerrainCollissionNode::computeVisiblePoints()
{
    // Make sure we have at least a triangle.
    assert(mConvexPoints.size() > 2);

    // Previous, current and the next point.
    std::list<Point>::iterator iPrev, iCurr, iNext;
    iPrev = mConvexPoints.end();
    iPrev--;
    iNext = mConvexPoints.begin();
    iNext++;
    for(auto iCurr = mConvexPoints.begin(); iCurr != mConvexPoints.end(); iCurr++)
    {
        for(auto it = iNext; it != mConvexPoints.end(); it++)
        {
            if(!convexAngleContains(iPrev->pos, iCurr->pos, iNext->pos, it->pos) && !isLineIntersecting(iCurr->pos, it->pos))
            {
                // Path curr->it
                mPaths.push_back(Path(&(*iCurr), &(*it)));
                iCurr->paths.push_back(&mPaths.back());

                // Path it->curr
                mPaths.push_back(Path(&(*it), &(*iCurr)));
                it->paths.push_back(&mPaths.back());
            }
        }

        iPrev++;
        if(iPrev == mConvexPoints.end())
            iPrev = mConvexPoints.begin();

        iNext++;
        if(iNext == mConvexPoints.end())
            break;
    }
}


// Compute visible points on other nodes.
void TerrainCollissionNode::computeVisiblePoints(std::list<std::unique_ptr<TerrainCollissionNode>>& nodes)
{
    for(Point& p1 : mConvexPoints)
    {
        for(const std::unique_ptr<TerrainCollissionNode>& pNode : nodes)
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


std::vector<sf::Vector2f> TerrainCollissionNode::computeConvexAngles()
{
    std::vector<sf::Vector2f> convexAngles;
    /*
     * If the number of points is not greater than
     * five, we only have convex angles. (First and
     * last points are the same)
     */
     if(mPoints.size() <= 5)
        convexAngles = mPoints;
     else
     {
        // Previous, current and the next point.
        sf::Vector2f prev, curr, next;

        // Examine the first angle.
        prev = mPoints[mPoints.size() - 2];
        curr = mPoints[0];
        next = mPoints[1];

        if(isAngleConvex(prev, curr, next))
            convexAngles.push_back(curr);

        // Now do the rest.
        for(unsigned int i = 1; i < mPoints.size() - 1; i++)
        {
            prev = mPoints[i - 1];
            curr = mPoints[i];
            next = mPoints[i + 1];

            if(isAngleConvex(prev, curr, next))
                convexAngles.push_back(curr);
        }
     }

    return convexAngles;
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

const std::list<TerrainCollissionNode::Point>& TerrainCollissionNode::getConvexAngles() const
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
