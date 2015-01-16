#include "CollissionFinder.hpp"

#include "Utility.hpp"

CollissionFinder::CollissionFinder(sf::FloatRect area)
: mQuadtree(area, mNodes)
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

std::list<CollissionFinder::CollissionData> CollissionFinder::getCollissions()
{
    std::list<std::pair<EntityNode*, EntityNode*>> nearbyEntities = mQuadtree.getNearbyEntities();

    std::list<CollissionData> collissions;
    for(auto pair : nearbyEntities)
    {
        float radiusSum = pair.first->getBoundingRect().width / 2 + pair.second->getBoundingRect().width / 2;

        sf::Vector2f dVec = pair.first->getPosition() - pair.second->getPosition();

        float dSqrd = dVec.x * dVec.x + dVec.y * dVec.y;

        if(dSqrd < radiusSum * radiusSum)
        {
            CollissionData collission;
            collission.lNode = pair.first;
            collission.rNode = pair.second;

            float d = sqrtf(dSqrd);
            collission.penetrationDepth = radiusSum - d;
            collission.unitVector = dVec / d;

            collissions.push_back(collission);
        }
    }

    return collissions;
}
