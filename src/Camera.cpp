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

#include "Camera.hpp"


////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Window/Event.hpp"
////////////////////////////////////////////////




#include "TIME_PER_FRAME.hpp"
#include "Utility.hpp"

Camera::Camera(sf::RenderWindow& window, sf::RenderTarget& target)
: mWindow(window)
, mTarget(target)
, mView(mTarget.getView())
, mSpeed(500.f)
, mZoom(1.f)
{

}

void Camera::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::MouseWheelMoved)
    {
        // If scrolling forwards, zoom in.
        if(event.mouseWheel.delta < 0)
            zoom(2.f);
        // If scrolling backwards, zoom out.
        else
            zoom(0.5f);
    }
}

void Camera::move(sf::Vector2f distance)
{
    mView.move(distance);
    mTarget.setView(mView);
}

void Camera::zoom(float factor)
{
    float zoom = mZoom * factor;
    if(zoom < 0.25f || zoom > 4.f)
        return;
    else
    {
        mZoom = zoom;
        mView.zoom(factor);
        mTarget.setView(mView);
    }
}

void Camera::update()
{
    sf::Vector2f direction(0, 0);
    sf::Vector2f velocity(0, 0);

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
        sf::Vector2f distance = direction * mSpeed * TIME_PER_FRAME::S;
        move(distance);
    }
    else
        moveToMouse();
}

void Camera::moveToMouse()
{
    sf::Vector2f mousePos = mTarget.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
    sf::Vector2f viewCenter = mView.getCenter();

    sf::Vector2f viewSize = mView.getSize();
    sf::FloatRect viewRect(viewCenter.x - viewSize.x / 2 + 1,
                          viewCenter.y - viewSize.y / 2 + 1,
                          viewSize.x - 5,
                          viewSize.y - 5);

    if(!intersects(mousePos, viewRect))
    {
        sf::Vector2f dVec =  mousePos - viewCenter;
        float d = sqrtf(dVec.x * dVec.x + dVec.y * dVec.y);
        sf::Vector2f unitVec = dVec / d;

        sf::Vector2f distance = unitVec * mSpeed * TIME_PER_FRAME::S;
        move(distance);
    }
}
