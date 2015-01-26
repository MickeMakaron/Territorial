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
        struct Point;
        struct Path;

        struct Point
        {
            sf::Vector2f        pos;
            std::list<Path*>    paths;
        };

        struct Path
        {
            Path(const Point* from, const Point* to, float passWidth = 200);
            float   passWidth;
            const Point*  p; ///< Destination point
            float   lengthSqrd;
            bool isEdge;
        };

        TerrainCollissionNode(const std::vector<sf::Vector2f>& points);
        TerrainCollissionNode();
        const Point* getClosestPoint(sf::Vector2f p, float* minSqrd = nullptr) const;
        virtual sf::FloatRect   getBoundingRect() const;
        void    setPoints(const std::vector<sf::Vector2f>& points);

        std::list<Point>& getConvexAngles();
        bool isLineIntersecting(sf::Vector2f a, sf::Vector2f b) const;
        bool isLineIntersecting(sf::Vector2f a, sf::Vector2f b, std::pair<const Point*, const Point*>& entry, std::pair<const Point*, const Point*>& exit) const;
        void connectVisiblePoints(std::unique_ptr<TerrainCollissionNode>& node, std::list<std::unique_ptr<TerrainCollissionNode>>& nodes);
        void connectPoints(Point& from, Point& to);
        bool convexAngleContains(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, sf::Vector2f p) const;
        std::pair<double, double> getMinMaxAngles(sf::Vector2f pos);

        void getVisiblePoints(sf::Vector2f p, const std::list<std::unique_ptr<TerrainCollissionNode>>& nodes, std::list<const Point*>& visiblePoints) const;
    private:

        void connectVisiblePoints();

        bool addSectorIfVisible(std::vector<std::pair<double, double>>& sectors, const std::pair<double, double>& sector);
        void mergeSectors(std::vector<std::pair<double, double>>& sectors);

        std::vector<sf::Vector2f> computeConvexAngles();
        void initializeConvexPoints(std::vector<sf::Vector2f> convexAngles);

        bool lineIntersects(sf::Vector2f p1, sf::Vector2f p2, const std::list<std::unique_ptr<TerrainCollissionNode>>& nodes) const;

        virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void            updateCurrent(CommandQueue&);

    private:
        PolygonShape mShape;
        std::vector<sf::Vector2f> mPoints;
        std::list<Point>        mConvexPoints;
        std::list<Path>         mPaths;


};

#endif // ANTGAME_TERRAINCOLLISSIONNODE_HPP
