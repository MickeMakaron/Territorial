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

#include "Map.hpp"
#include "Utility.hpp"

#include <cassert>

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////


Map::Map(const std::string& filePath)
{
    load(filePath);

    std::vector<sf::Vector2f> points =
    {
        sf::Vector2f(100, 100),
        sf::Vector2f(200, 150),
        sf::Vector2f(300, 100),
        sf::Vector2f(250, 200),
        sf::Vector2f(150, 150),
        sf::Vector2f(100, 100),
    };

    mTCN.setPoints(points);
}


std::list<const TerrainCollissionNode*> Map::getImpassableTerrain() const
{
    return std::list<const TerrainCollissionNode*>({&mTCN});
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mDrawShape);
    target.draw(mTCN);
    mTCN.drawBoundingRect(target, states);
}

sf::FloatRect Map::getBounds() const
{
    return mDrawShape.getGlobalBounds();
}

void Map::load(const std::string& filePath)
{
    mTexture.loadFromFile(filePath);
    sf::Vector2f texSize(mTexture.getSize().x, mTexture.getSize().y);

    mDrawShape.setSize(texSize);
    mDrawShape.setTexture(&mTexture);
    mDrawShape.setScale(8.f, 8.f);

    mDrawShape.move(-500, -500);
}

