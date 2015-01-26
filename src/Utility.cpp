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

#include <Utility.hpp>

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <random>
#include <cmath>
#include <ctime>
#include <cassert>
////////////////////////////////////////////////

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Shape.hpp"
////////////////////////////////////////////////

namespace
{
	std::default_random_engine createRandomEngine()
	{
		auto seed = static_cast<unsigned long>(std::time(nullptr));
		return std::default_random_engine(seed);
	}

	auto RandomEngine = createRandomEngine();
}


std::string toString(sf::Keyboard::Key key)
{
	#define GAME_KEYTOSTRING_CASE(KEY) case sf::Keyboard::KEY: return #KEY;

	switch (key)
	{
		GAME_KEYTOSTRING_CASE(Unknown)
		GAME_KEYTOSTRING_CASE(A)
		GAME_KEYTOSTRING_CASE(B)
		GAME_KEYTOSTRING_CASE(C)
		GAME_KEYTOSTRING_CASE(D)
		GAME_KEYTOSTRING_CASE(E)
		GAME_KEYTOSTRING_CASE(F)
		GAME_KEYTOSTRING_CASE(G)
		GAME_KEYTOSTRING_CASE(H)
		GAME_KEYTOSTRING_CASE(I)
		GAME_KEYTOSTRING_CASE(J)
		GAME_KEYTOSTRING_CASE(K)
		GAME_KEYTOSTRING_CASE(L)
		GAME_KEYTOSTRING_CASE(M)
		GAME_KEYTOSTRING_CASE(N)
		GAME_KEYTOSTRING_CASE(O)
		GAME_KEYTOSTRING_CASE(P)
		GAME_KEYTOSTRING_CASE(Q)
		GAME_KEYTOSTRING_CASE(R)
		GAME_KEYTOSTRING_CASE(S)
		GAME_KEYTOSTRING_CASE(T)
		GAME_KEYTOSTRING_CASE(U)
		GAME_KEYTOSTRING_CASE(V)
		GAME_KEYTOSTRING_CASE(W)
		GAME_KEYTOSTRING_CASE(X)
		GAME_KEYTOSTRING_CASE(Y)
		GAME_KEYTOSTRING_CASE(Z)
		GAME_KEYTOSTRING_CASE(Num0)
		GAME_KEYTOSTRING_CASE(Num1)
		GAME_KEYTOSTRING_CASE(Num2)
		GAME_KEYTOSTRING_CASE(Num3)
		GAME_KEYTOSTRING_CASE(Num4)
		GAME_KEYTOSTRING_CASE(Num5)
		GAME_KEYTOSTRING_CASE(Num6)
		GAME_KEYTOSTRING_CASE(Num7)
		GAME_KEYTOSTRING_CASE(Num8)
		GAME_KEYTOSTRING_CASE(Num9)
		GAME_KEYTOSTRING_CASE(Escape)
		GAME_KEYTOSTRING_CASE(LControl)
		GAME_KEYTOSTRING_CASE(LShift)
		GAME_KEYTOSTRING_CASE(LAlt)
		GAME_KEYTOSTRING_CASE(LSystem)
		GAME_KEYTOSTRING_CASE(RControl)
		GAME_KEYTOSTRING_CASE(RShift)
		GAME_KEYTOSTRING_CASE(RAlt)
		GAME_KEYTOSTRING_CASE(RSystem)
		GAME_KEYTOSTRING_CASE(Menu)
		GAME_KEYTOSTRING_CASE(LBracket)
		GAME_KEYTOSTRING_CASE(RBracket)
		GAME_KEYTOSTRING_CASE(SemiColon)
		GAME_KEYTOSTRING_CASE(Comma)
		GAME_KEYTOSTRING_CASE(Period)
		GAME_KEYTOSTRING_CASE(Quote)
		GAME_KEYTOSTRING_CASE(Slash)
		GAME_KEYTOSTRING_CASE(BackSlash)
		GAME_KEYTOSTRING_CASE(Tilde)
		GAME_KEYTOSTRING_CASE(Equal)
		GAME_KEYTOSTRING_CASE(Dash)
		GAME_KEYTOSTRING_CASE(Space)
		GAME_KEYTOSTRING_CASE(Return)
		GAME_KEYTOSTRING_CASE(BackSpace)
		GAME_KEYTOSTRING_CASE(Tab)
		GAME_KEYTOSTRING_CASE(PageUp)
		GAME_KEYTOSTRING_CASE(PageDown)
		GAME_KEYTOSTRING_CASE(End)
		GAME_KEYTOSTRING_CASE(Home)
		GAME_KEYTOSTRING_CASE(Insert)
		GAME_KEYTOSTRING_CASE(Delete)
		GAME_KEYTOSTRING_CASE(Add)
		GAME_KEYTOSTRING_CASE(Subtract)
		GAME_KEYTOSTRING_CASE(Multiply)
		GAME_KEYTOSTRING_CASE(Divide)
		GAME_KEYTOSTRING_CASE(Left)
		GAME_KEYTOSTRING_CASE(Right)
		GAME_KEYTOSTRING_CASE(Up)
		GAME_KEYTOSTRING_CASE(Down)
		GAME_KEYTOSTRING_CASE(Numpad0)
		GAME_KEYTOSTRING_CASE(Numpad1)
		GAME_KEYTOSTRING_CASE(Numpad2)
		GAME_KEYTOSTRING_CASE(Numpad3)
		GAME_KEYTOSTRING_CASE(Numpad4)
		GAME_KEYTOSTRING_CASE(Numpad5)
		GAME_KEYTOSTRING_CASE(Numpad6)
		GAME_KEYTOSTRING_CASE(Numpad7)
		GAME_KEYTOSTRING_CASE(Numpad8)
		GAME_KEYTOSTRING_CASE(Numpad9)
		GAME_KEYTOSTRING_CASE(F1)
		GAME_KEYTOSTRING_CASE(F2)
		GAME_KEYTOSTRING_CASE(F3)
		GAME_KEYTOSTRING_CASE(F4)
		GAME_KEYTOSTRING_CASE(F5)
		GAME_KEYTOSTRING_CASE(F6)
		GAME_KEYTOSTRING_CASE(F7)
		GAME_KEYTOSTRING_CASE(F8)
		GAME_KEYTOSTRING_CASE(F9)
		GAME_KEYTOSTRING_CASE(F10)
		GAME_KEYTOSTRING_CASE(F11)
		GAME_KEYTOSTRING_CASE(F12)
		GAME_KEYTOSTRING_CASE(F13)
		GAME_KEYTOSTRING_CASE(F14)
		GAME_KEYTOSTRING_CASE(F15)
		GAME_KEYTOSTRING_CASE(Pause)
		default:
		    return "";
            break;
	}
}

void centerOrigin(sf::Sprite& sprite)
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
}

void centerOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
}

void centerOrigin(sf::Shape& shape)
{
    sf::FloatRect bounds = shape.getLocalBounds();
    shape.setOrigin(std::floor(bounds.width/2.f), std::floor(bounds.height/2.f));
}

float toDegree(float radian)
{
	return 180.f / 3.141592653589793238462643383f * radian;
}

float toRadian(float degree)
{
	return 3.141592653589793238462643383f / 180.f * degree;
}

int randomInt(int exclusiveMax)
{
	std::uniform_int_distribution<> distr(0, exclusiveMax - 1);
	return distr(RandomEngine);
}

float length(sf::Vector2f vector)
{
	return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

sf::Vector2f unitVector(sf::Vector2f vector)
{
	assert(vector != sf::Vector2f(0.f, 0.f));
	return vector / length(vector);
}

bool contains(sf::FloatRect lhs, sf::FloatRect rhs)
{
    if(lhs.left < rhs.left)
        return false;

    if(lhs.top < rhs.top)
        return false;

    if(lhs.left + lhs.width > rhs.left + rhs.width)
        return false;

    if(lhs.top + lhs.height > rhs.top + rhs.height)
        return false;

    return true;
}

bool intersects(sf::FloatRect lhs, sf::FloatRect rhs)
{
    bool xIsect = false;
    bool yIsect = false;

    float lRight, lBottom, rRight, rBottom;
    lRight = lhs.left + lhs.width;
    lBottom = lhs.top + lhs.height;
    rRight = rhs.left + rhs.width;
    rBottom = rhs.top + rhs.height;

    if(lhs.left >= rhs.left && lhs.left <= rRight)
        xIsect = true;
    else if(lRight <= rRight && lRight >= rhs.left)
        xIsect = true;
    else
        return false;

    if(lhs.top >= rhs.top && lhs.top <= rBottom)
        yIsect = true;
    else if(lBottom <= rBottom && lBottom >= rhs.top)
        yIsect = true;
    else
        return false;

    return xIsect && yIsect;
}

bool intersects(sf::Vector2f p, sf::FloatRect rect)
{
    if(p.x < rect.left)
        return false;
    else if(p.y < rect.top)
        return false;
    else if(p.x > rect.left + rect.width)
        return false;
    else if(p.y > rect.top + rect.height)
        return false;
    else
        return true;
}

bool intersects(sf::Vector2f a1, sf::Vector2f a2, sf::Vector2f b1, sf::Vector2f b2, sf::Vector2f* intersection)
{
    //////////////////////////
    // l - left hand line,  //
    // r - right hand line, //
    // 1 - first in pair,   //
    // 2 - second in pair,  //
    // X - X-value,         //
    // Y - Y-value,         //
    //////////////////////////
    float   l1X = a1.x,
            l1Y = a1.y,

            l2X = a2.x,
            l2Y = a2.y,

            r1X = b1.x,
            r1Y = b1.y,

            r2X = b2.x,
            r2Y = b2.y;

    // Check if intersection is possible at all
    if (std::min(l1X, l2X) > std::max(r1X, r2X))
        return false;
    if (std::max(l1X, l2X) < std::min(r1X,r2X))
        return false;
    if (std::min(l1Y, l2Y) > std::max(r1Y,r2Y))
        return false;
    if (std::max(l1Y, l2Y) < std::min(r1Y,r2Y))
        return false;



    float d = (r2Y - r1Y)*(l2X - l1X) - (r2X - r1X)*(l2Y - l1Y);
    if(std::fabs(d) < 0.00001)
        return false;

    float nL = ((r2X - r1X)*(l1Y - r1Y) - (r2Y - r1Y)*(l1X - r1X))/d;
    if(nL < 0 || nL > 1.f)
        return false;

    float nR = ((l2X - l1X)*(l1Y - r1Y) - (l2Y - l1Y)*(l1X - r1X))/d;
    if(nR < 0 || nR > 1.f)
        return false;

    if(intersection)
        *intersection = sf::Vector2f(l1X + nL*(l2X - l1X),
                                         l1Y + nL*(l2Y - l1Y));

    return true;

/*
    sf::Vector2f d = a1 - a2;

    // Get arbitrary point to the left to check with.
    sf::Vector2f lPoint = a1 + sf::Vector2f(d.y, -d.x);

    // Get arbitrary point to the right to check with.
    sf::Vector2f rPoint = a1 + sf::Vector2f(-d.y, d.x);

    bool b1IsRight, b2IsRight;

    b1IsRight = lengthSqrd(b1 - rPoint) < lengthSqrd(b1 - lPoint) ? true : false;
    b2IsRight = lengthSqrd(b2 - rPoint) < lengthSqrd(b2 - lPoint) ? true : false;

    if((b1IsRight && !b2IsRight) || (!b1IsRight && b2IsRight))
    {
        int flerp = 0;
    }

    return (b1IsRight && !b2IsRight) || (!b1IsRight && b2IsRight);
*/

}


/**
 * a - b
 *     |
 *     c
 */
bool isAngleConvex(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c)
{
     /*
     * What this does is simply check which side of the line a->b the next point c is.
     * Since shapes are defined in a clockwise order, it must be on the right side.
     * It works by establishing two points that are guaranteed to the left and right
     * of the line. Which of these two points c is closest to determines which side of
     * the line c is on. If c is closer to the right pointer, it is on the right side
     * and vice versa.
     */
    sf::Vector2f d = b - a;

    // Get arbitrary point to the left to check with.
    sf::Vector2f lPoint = b + sf::Vector2f(d.y, -d.x);

    // Get arbitrary point to the right to check with.
    sf::Vector2f rPoint = b + sf::Vector2f(-d.y, d.x);

    return lengthSqrd(c - rPoint) < lengthSqrd(c - lPoint);
}




const float PX_PER_M = 20.f;
const float M_PER_PX = 1/20.f;

float toMetres(float px)
{
    return px*M_PER_PX;
}

sf::Vector2f toMetres(sf::Vector2f px)
{
    return px*M_PER_PX;
}

float toPixels(float m)
{
    return m*PX_PER_M;
}

sf::Vector2f toPixels(sf::Vector2f m)
{
    return m*PX_PER_M;
}

sf::Vector2f abs(sf::Vector2f v)
{
    return sf::Vector2f(std::fabs(v.x), std::fabs(v.y));
}

float dot(sf::Vector2f a, sf::Vector2f b)
{
    return a.x*b.x + a.y*b.y;
}

sf::Vector2f distance(const sf::Vector2f lhs, const sf::Vector2f rhs)
{
    return rhs - lhs;
}

float lengthSqrd(sf::Vector2f v)
{
    return v.x*v.x + v.y*v.y;
}
