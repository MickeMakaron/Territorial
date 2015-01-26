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

#ifndef GAME_UTILITY_HPP
#define GAME_UTILITY_HPP

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Window/Keyboard.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Rect.hpp"
////////////////////////////////////////////////


#include <sstream>

#define PI 3.14159f

namespace sf
{
	class Sprite;
	class Text;
	class Shape;
}

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string		toString(const T& value);

// Convert enumerators to strings
std::string		toString(sf::Keyboard::Key key);

// Call setOrigin() with the center of the object
void			centerOrigin(sf::Sprite& sprite);
void			centerOrigin(sf::Text& text);
void            centerOrigin(sf::Shape& shape);

// Degree/radian conversion
float			toDegree(float radian);
float			toRadian(float degree);

// Pixel/Meter conversion
float           toMetres(float px);
sf::Vector2f    toMetres(sf::Vector2f px);

float           toPixels(float m);
sf::Vector2f    toPixels(sf::Vector2f m);


sf::Vector2f    distance(const sf::Vector2f lhs, const sf::Vector2f rhs);
float           lengthSqrd(sf::Vector2f v);

// Random number generation
int				randomInt(int exclusiveMax);

// Vector operations
float			length(sf::Vector2f vector);
sf::Vector2f	unitVector(sf::Vector2f vector);
sf::Vector2f    abs(sf::Vector2f v);
float           dot(sf::Vector2f a, sf::Vector2f b);



bool    contains(sf::FloatRect lhs, sf::FloatRect rhs);
bool    intersects(sf::FloatRect lhs, sf::FloatRect rhs);
bool    intersects(sf::Vector2f p, sf::FloatRect rect);
bool    intersects(sf::Vector2f a1, sf::Vector2f a2, sf::Vector2f b1, sf::Vector2f b2, sf::Vector2f* intersection = nullptr);

bool    isAngleConvex(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c);

#include <Utility.inl>
#endif // GAME_UTILITY_HPP
