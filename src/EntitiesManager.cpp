#include "EntitiesManager.hpp"


#include "CommandQueue.hpp"




EntitiesManager::EntitiesManager(sf::FloatRect mapRect, CommandQueue& commandQueue)
: mCommandQueue(commandQueue)
, mCollissionManager(mapRect)
{

}

void EntitiesManager::insertEntity(std::unique_ptr<EntityNode> entity)
{
    mCollissionManager.insertEntity(entity.get());
    mEntitiesGraph.attachChild(std::move(entity));
}

void EntitiesManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mEntitiesGraph);

    // Quadtree debugging
    sf::RectangleShape shape;
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);

    std::list<const Quadtree*> quadtree = mCollissionManager.getQuadtree();
    for(const Quadtree* quad : quadtree)
    {
        assert(quad != nullptr);

        sf::FloatRect bounds = quad->getBoundingRect();
        shape.setPosition(bounds.left, bounds.top);
        shape.setSize(sf::Vector2f(bounds.width, bounds.height));

        target.draw(shape);
    }
}

void EntitiesManager::removeWrecks()
{
    mCollissionManager.removeWrecks();
    mEntitiesGraph.removeWrecks();
}


void EntitiesManager::update()
{
    while (!mCommandQueue.isEmpty())
		mEntitiesGraph.onCommand(mCommandQueue.pop());

    mEntitiesGraph.update(mCommandQueue);
    mCollissionManager.update();
}
