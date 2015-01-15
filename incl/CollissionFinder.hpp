#ifndef ANTGAME_COLLISSIONFINDER_HPP
#define ANTGAME_COLLISSIONFINDER_HPP

#include "Quadtree.hpp"

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <list>
////////////////////////////////////////////////


class CollissionFinder
{
    public:
        CollissionFinder(sf::Vector2u mapSize);

        void    update();

        std::list<Quadtree*>& getQuadtree();
        void  insertEntity(EntityNode* entity);
        std::list<std::pair<EntityNode*, EntityNode*>> getCollissions();

    private:
        std::list<Quadtree*>    mBottomQuads;
        Quadtree                mQuadtree;
        std::list<Quadtree::Node>         mNodes;
};

#endif //ANTGAME_COLLISSIONFINDER_HPP
