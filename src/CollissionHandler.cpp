#include "CollissionHandler.hpp"

#include "Utility.hpp"

CollissionHandler::CollissionHandler(sf::FloatRect area)
: mCollissionFinder(area)
{

}

void CollissionHandler::update()
{
    std::list<CollissionData> collissions = mCollissionFinder.getCollissions();

    for(CollissionFinder::CollissionData& collission : collissions)
    {
        collission.lNode->goTo(collission.lNode->getPosition() + collission.unitVector * 10.f);
        collission.rNode->goTo(collission.rNode->getPosition() - collission.unitVector * 10.f);
    }
}

void CollissionHandler::insert(EntityNode* entity)
{
    mCollissionFinder.insertEntity(entity);
}


