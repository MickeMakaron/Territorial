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

#ifndef ANTGAME_QUADTREE_HPP
#define ANTGAME_QUADTREE_HPP

#include <EntityNode.hpp>

////////////////////////////////////////////////
// C++ Standard Library
#include <list>
#include <set>
////////////////////////////////////////////////

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics.hpp>
////////////////////////////////////////////////



class Quadtree
{
    public:
        struct Node
        {
            std::list<Quadtree*>    quads;
            EntityNode*             entity;
        };

                Quadtree(sf::FloatRect bounds, std::list<Node>& nodes);

        void    update();
        void    insertEntity(EntityNode* entity);
        void eraseEntity(EntityNode* entity);

        /////////////////////////////////////////////////////////
        // For testing purposes
        void    getQuadtree(std::list<const Quadtree*>& qtree) const;
        /////////////////////////////////////////////////////////

        std::set<std::pair<EntityNode*, EntityNode*>> getNearbyEntities();

        sf::FloatRect getBoundingRect() const;
        void    removeWrecks();

    private:
                Quadtree(int level, sf::FloatRect bounds, std::list<Node>& nodes);

        void            updateTree();
        void            updateNodes(std::list<Node*>& updatedNodes);


        void            insertNode(Node* node);

        void            getBottomQuads(std::list<Quadtree*>& quads);

        void            eraseNode(std::list<Node>::iterator iNode);
        void            eraseQuadNode(Node* node);
        void            clear();

        void            split();
        void            merge();

        bool            isOutOfBounds(EntityNode* node);
        unsigned char   getPartialIndices(sf::FloatRect nodeBounds);




        bool                hasChildren();
        std::list<Node*>    getQuadNodes();

    private:
        unsigned int MAX_NODES = 5;
        unsigned int MAX_LEVELS = 10;
        unsigned int mLevel;

        std::vector<Quadtree>   mChildren;
        std::list<Node*>        mQuadNodes;
        sf::FloatRect           mBounds;

        std::list<Node>&        mNodes;
};

#endif //ANTGAME_QUADTREE_HPP
