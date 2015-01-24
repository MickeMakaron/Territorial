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



void TerrainCollissionNode::computeConvexAngles()
{
    /*
     * If the number of points is not greater than
     * five, we only have convex angles. (First and
     * last points are the same)
     */
     if(mPoints.size() <= 5)
     {
        mConvexAngles = mPoints;
        return;
     }

    // Previous, current and the next point.
    sf::Vector2f prev, curr, next;

    // Examine the first angle.
    prev = mPoints[mPoints.size() - 2];
    curr = mPoints[0];
    next = mPoints[1];

    mConvexAngles.clear();
    if(isAngleConvex(prev, curr, next))
        mConvexAngles.push_back(curr);

    // Now do the rest.
    for(unsigned int i = 1; i < mPoints.size() - 1; i++)
    {
        prev = mPoints[i - 1];
        curr = mPoints[i];
        next = mPoints[i + 1];

        if(isAngleConvex(prev, curr, next))
            mConvexAngles.push_back(curr);
    }
}

bool TerrainCollissionNode::isLineIntersecting(sf::Vector2f a, sf::Vector2f b) const
{
    for(unsigned int i = 0; i < mPoints.size() - 1; i++)
        if(intersects(mPoints[i], mPoints[i + 1], a, b))
            return true;
}

const std::vector<sf::Vector2f>& TerrainCollissionNode::getConvexAngles() const
{
    return mConvexAngles;
}

void TerrainCollissionNode::updateCurrent(CommandQueue&)
{

}

sf::Vector2f TerrainCollissionNode::getPoint(unsigned int index) const
{
    return mShape.getPoint(index);
}

void TerrainCollissionNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape);
}

sf::FloatRect TerrainCollissionNode::getBoundingRect() const
{
    return mShape.getGlobalBounds();
}
