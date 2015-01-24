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

#ifndef ANTGAME_POLYGONSHAPE_HPP
#define ANTGAME_POLYGONSHAPE_HPP

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/Shape.hpp"
#include "SFML/Graphics/VertexArray.hpp"
////////////////////////////////////////////////

class PolygonShape : public sf::Shape
{
    public:
        PolygonShape(const std::vector<sf::Vector2f>& points, sf::Color color = sf::Color::Black);
        PolygonShape(sf::Color color = sf::Color::Black);

        virtual unsigned int    getPointCount() const;
        virtual sf::Vector2f    getPoint(unsigned int index) const;
        virtual sf::FloatRect   getGlobalBounds() const;

        virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void    setColor(sf::Color color);
        void    setPoints(const std::vector<sf::Vector2f>& points);

    private:
        sf::VertexArray mPoints;
        sf::FloatRect   mBounds;

        sf::Color       mColor;
};

#endif // ANTGAME_POLYGONSHAPE_HPP
