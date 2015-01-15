#include "CollissionFinder.hpp"

CollissionFinder::CollissionFinder(sf::Vector2u mapSize)
: mQuadtree(sf::FloatRect(0, 0, mapSize.x, mapSize.y), mNodes)
{

}

void CollissionFinder::update()
{
    mQuadtree.update();
}

std::list<Quadtree*>& CollissionFinder::getQuadtree()
{
    mBottomQuads.clear();
    mQuadtree.getQuadtree(mBottomQuads);

    return mBottomQuads;
}

void CollissionFinder::insertEntity(EntityNode* entity)
{
    if(!entity->isMarkedForRemoval())
        mQuadtree.insertEntity(entity);
}

std::list<std::pair<EntityNode*, EntityNode*>> CollissionFinder::getCollissions()
{
    return std::move(mQuadtree.getNearbyEntities());
}
