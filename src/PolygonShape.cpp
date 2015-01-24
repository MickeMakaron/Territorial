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

#include <PolygonShape.hpp>

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////


PolygonShape::PolygonShape(const std::vector<sf::Vector2f>& points, sf::Color color)
: mColor(color)
, mPoints(sf::LinesStrip, 0)
{
    setPoints(points);
}

PolygonShape::PolygonShape(sf::Color color)
: mColor(color)
, mPoints(sf::LinesStrip, 0)
{

}

void PolygonShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mPoints, states);
}

void PolygonShape::setPoints(const std::vector<sf::Vector2f>& points)
{
    if(points.size() == 0)
    {
        mPoints.resize(points.size());
        return;
    }

    mPoints.resize(points.size());

    sf::Vertex v;
    v.color = mColor;
    for(int i = 0; i < points.size(); i++)
    {
        v.position = points[i];
        mPoints[i] = v;
    }
}

void PolygonShape::setColor(sf::Color color)
{
    for(int i = 0; i < mPoints.getVertexCount(); i++)
        mPoints[i].color = color;
}

unsigned int PolygonShape::getPointCount() const
{
    return mPoints.getVertexCount();
}

sf::Vector2f PolygonShape::getPoint(unsigned int index) const
{
    return mPoints[index].position;
}

sf::FloatRect PolygonShape::getGlobalBounds() const
{
    return mPoints.getBounds();
}
