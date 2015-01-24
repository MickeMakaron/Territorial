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
#include "Map.hpp"
#include "Utility.hpp"

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////


////////////////////////////////////////////////
// STD - C++ Standard Library
#include <cmath>
////////////////////////////////////////////////

Pathfinder::Pathfinder(const Map& map)
: mImpassableTerrain(map.getImpassableTerrain())
{

}

void Pathfinder::draw(sf::RenderTarget& target) const
{
    target.draw(mDebug);
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

std::list<Pathfinder::Waypoint> Pathfinder::getPath(sf::Vector2f pos, sf::Vector2f destination)
{
    std::list<Waypoint> wayPoints;
    std::vector<sf::Vector2f> points;

    sf::Vector2f dVec = destination - pos;
    float rotation = std::atan(dVec.y/dVec.x);
    double s = std::sin(-rotation);
    double c = std::cos(-rotation);

    for(const TerrainCollissionNode* tcn : mImpassableTerrain)
    {
        if(lineIntersectsRect(pos, destination, tcn->getBoundingRect()))
            if(tcn->isLineIntersecting(pos, destination))
            {
                std::vector<sf::Vector2f> convexAngles = tcn->getConvexAngles();

                unsigned int iMin = 0;
                unsigned int iMax = 0;

                float minY = convexAngles[iMin].x * s + convexAngles[iMin].y * c;
                float maxY = minY;

                for(int i = 0; i < convexAngles.size(); i++)
                {
                    float rotatedY = convexAngles[i].x * s + convexAngles[i].y * c;
                    if(rotatedY < minY)
                    {
                        iMin = i;
                        minY = rotatedY;
                    }
                    else if(rotatedY > maxY)
                    {
                        iMax = i;
                        maxY = rotatedY;
                    }
                }






                float minLengthSqrd = lengthSqrd(convexAngles[iMin] - pos) + lengthSqrd(destination - convexAngles[iMin]);
                float maxLengthSqrd = lengthSqrd(convexAngles[iMax] - pos) + lengthSqrd(destination - convexAngles[iMax]);

                if(minLengthSqrd < maxLengthSqrd)
                    points.push_back(convexAngles[iMin]);
                else
                    points.push_back(convexAngles[iMax]);
            }
    }




    points.push_back(destination);


    sf::Vector2f prev = pos;
    for(sf::Vector2f p : points)
    {
        Waypoint wp;
        wp.destination = p;

        dVec = p - prev;
        float d = sqrt(dVec.x * dVec.x + dVec.y * dVec.y);

        wp.distance = d;
        wp.direction = dVec / d;

        prev = p;
        wayPoints.push_back(wp);
    }

    return wayPoints;
}