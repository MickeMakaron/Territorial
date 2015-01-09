#ifndef GAME_UTILITY_HPP
#define GAME_UTILITY_HPP

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <sstream>


namespace sf
{
	class Sprite;
	class Text;
	class Shape;
}

class Animation;
class Polygon;

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

#include <Utility.inl>
#endif // GAME_UTILITY_HPP
