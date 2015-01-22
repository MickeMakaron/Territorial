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

#ifndef ANTGAME_CAMERA_HPP
#define ANTGAME_CAMERA_HPP


////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
namespace sf
{
    class Event;
}
////////////////////////////////////////////////



class Camera
{
    public:
        /**
         * \brief Constructor
         */
        Camera(sf::RenderWindow& window, sf::RenderTarget& target);

        /**
         * \brief Update camera
         */
        void update();


        void handleEvent(const sf::Event& event);


    private:
        void move(sf::Vector2f distance);
        void zoom(float factor);
        void moveToMouse();

    private:
        sf::RenderWindow& mWindow; ///< SFML Window object.
        sf::RenderTarget& mTarget; ///< SFML rendering target to assign view to.
        sf::View mView; ///< SFML View object.
        float mSpeed; ///< SFML speed of camera when moving.
        float mZoom;
};


#endif // ANTGAME_CAMERA_HPP
