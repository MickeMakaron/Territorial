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

#include "Map.hpp"
#include "Utility.hpp"

#include <cassert>

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////


Map::Map(const std::string& filePath)
{
    load(filePath);

    buildMap();
}

Map::Map(const std::string& filePath, sf::Vector2f size)
{
    load(filePath);
    mDrawShape.setSize(size);

    buildMap();
}


void Map::buildMap()
{
    std::vector<sf::Vector2f> points =
    {
        sf::Vector2f(100, 100),
        sf::Vector2f(200, 150),
        sf::Vector2f(300, 100),
        sf::Vector2f(250, 200),
        sf::Vector2f(150, 150),
        sf::Vector2f(100, 100),
    };

    NodePtr node(new TerrainCollissionNode(points));

    mImpassableNodes.push_back(std::move(node));

    for(unsigned int x = 0; x < 10; x++)
    {
        for(unsigned int y = 0; y < 10; y++)
        {
            for(auto& p : points)
                p += sf::Vector2f(300, 0);

            node = NodePtr(new TerrainCollissionNode(points));
            mImpassableNodes.push_back(std::move(node));
        }

        for(auto& p : points)
            p += sf::Vector2f(-300 * 10, 70);
    }

    //for(NodePtr& pNode : mImpassableNodes)
     //   pNode->connectVisiblePoints(pNode, mImpassableNodes);


    for(auto i = mImpassableNodes.begin(); i != mImpassableNodes.end(); i++)
    {
        auto j = i;
        while(j != mImpassableNodes.end())
        {
            (*i)->connectVisiblePoints(*j, mImpassableNodes);
            j++;
        }
    }

    for(NodePtr& pNode : mImpassableNodes)
        pNode->computePassWidths(mImpassableNodes);


    mPaths.setPrimitiveType(sf::Lines);

    sf::Vertex p1, p2;
    p1.color = sf::Color::Red;
    p2.color = p1.color;

    for(NodePtr& pNode : mImpassableNodes)
        for(const TerrainCollissionNode::Point& point : pNode->getConvexAngles())
        {
            p1.position = point.pos;
            for(const TerrainCollissionNode::Path* pPath : point.paths)
            {
                p2.position = pPath->p->pos;

                mPaths.append(p1);
                mPaths.append(p2);
            }
        }

}


std::list<const TerrainCollissionNode::Point*> Map::getVisiblePoints(sf::Vector2f p) const
{
    std::list<const TerrainCollissionNode::Point*> visiblePoints;
    for(const NodePtr& pNode : mImpassableNodes)
        pNode->getVisiblePoints(p, mImpassableNodes, visiblePoints);

    assert(!visiblePoints.empty());
    return visiblePoints;
}

const std::list<Map::NodePtr>& Map::getImpassableTerrain() const
{
    return mImpassableNodes;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mDrawShape);

    for(const NodePtr& pNode : mImpassableNodes)
    {
        target.draw(*pNode.get());
        //pNode->drawBoundingRect(target, states);
    }

    target.draw(mPaths);
}



sf::FloatRect Map::getBounds() const
{
    return mDrawShape.getGlobalBounds();
}

void Map::load(const std::string& filePath)
{
    mTexture.loadFromFile(filePath);
    sf::Vector2f texSize(mTexture.getSize().x, mTexture.getSize().y);

    mDrawShape.setSize(texSize);
    mDrawShape.setTexture(&mTexture);
    mDrawShape.setScale(8.f, 8.f);

    mDrawShape.move(-500, -500);
}

