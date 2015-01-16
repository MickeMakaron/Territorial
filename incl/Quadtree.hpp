#ifndef ANTGAME_QUADTREE_HPP
#define ANTGAME_QUADTREE_HPP

#include <EntityNode.hpp>

////////////////////////////////////////////////
// C++ Standard Library
#include <list>
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

        /////////////////////////////////////////////////////////
        // For testing purposes
        void    getQuadtree(std::list<Quadtree*>& qtree);
        /////////////////////////////////////////////////////////

        std::list<std::pair<EntityNode*, EntityNode*>> getNearbyEntities();

        sf::FloatRect getBoundingRect();

    private:
                Quadtree(int level, sf::FloatRect bounds, std::list<Node>& nodes);

        void            updateTree();
        void            updateNodes(std::list<Node*>& updatedNodes);


        void            insertNode(Node* node);

        void            removeWrecks();
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
        unsigned int MAX_LEVELS = 5;
        unsigned int mLevel;

        std::vector<Quadtree>   mChildren;
        std::list<Node*>        mQuadNodes;
        sf::FloatRect           mBounds;

        std::list<Node>&        mNodes;
};

#endif //ANTGAME_QUADTREE_HPP
