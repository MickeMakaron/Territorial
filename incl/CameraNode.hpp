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

#ifndef ANTGAME_CAMERANODE_HPP
#define ANTGAME_CAMERANODE_HPP


////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////

#include "SceneNode.hpp"


class CameraNode : public SceneNode
{
    public:
        CameraNode(sf::RenderTarget& target);

        virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

    private:
        sf::RenderTarget& mTarget;
        sf::View mView;
};


#endif // ANTGAME_CAMERANODE_HPP
