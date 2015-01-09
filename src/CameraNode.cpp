#include "CameraNode.hpp"


////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Window/Keyboard.hpp"
////////////////////////////////////////////////


CameraNode::CameraNode(sf::RenderTarget& target)
: mTarget(target)
, mView(mTarget.getView())
{

}

void CameraNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    sf::Vector2f direction(0, 0);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        direction.x = -1;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        direction.x = 1;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        direction.y = -1;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        direction.y = 1;

    if(direction.x || direction.y)
    {
        sf::Vector2f distance = direction * 100.f * dt.asSeconds();
        mView.move(distance);
        mTarget.setView(mView);
    }
}
