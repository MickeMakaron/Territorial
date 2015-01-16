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
        struct CollissionData
        {
            EntityNode* lNode;
            EntityNode* rNode;

            float           penetrationDepth;
            sf::Vector2f    unitVector;
        };

        CollissionFinder(sf::FloatRect area);

        void    update();

        std::list<Quadtree*>& getQuadtree();
        void  insertEntity(EntityNode* entity);
        std::list<CollissionData> getCollissions();

    private:
        std::list<Quadtree*>        mBottomQuads;

        Quadtree                    mQuadtree;
        std::list<Quadtree::Node>   mNodes;
};

#endif //ANTGAME_COLLISSIONFINDER_HPP
