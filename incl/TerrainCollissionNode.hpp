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


#ifndef ANTGAME_TERRAINCOLLISSIONNODE_HPP
#define ANTGAME_TERRAINCOLLISSIONNODE_HPP

#include <SceneNode.hpp>
#include <PolygonShape.hpp>


class TerrainCollissionNode : public SceneNode
{
    public:
        TerrainCollissionNode(const std::vector<sf::Vector2f>& points);
        TerrainCollissionNode();

        virtual sf::FloatRect   getBoundingRect() const;
        void    setPoints(const std::vector<sf::Vector2f>& points);

        const std::vector<sf::Vector2f>& getConvexAngles() const;
        bool isLineIntersecting(sf::Vector2f a, sf::Vector2f b) const;


sf::Vector2f getPoint(unsigned int index) const;

    private:
        void                    computeConvexAngles();

        virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void            updateCurrent(CommandQueue&);

    private:
        PolygonShape mShape;
        std::vector<sf::Vector2f> mPoints;
        std::vector<sf::Vector2f> mConvexAngles;
};

#endif // ANTGAME_TERRAINCOLLISSIONNODE_HPP
