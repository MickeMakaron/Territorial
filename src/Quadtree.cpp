#include "Quadtree.hpp"
#include <Utility.hpp>




////////////////////////////////////////////////
// For debugging
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
////////////////////////////////////////////////

Quadtree::Quadtree(sf::FloatRect bounds, std::list<Node>& nodes)
: mLevel(0)
, mBounds(bounds)
, mNodes(nodes)
{
}

Quadtree::Quadtree(int level, sf::FloatRect bounds, std::list<Node>& nodes)
: mLevel(level)
, mBounds(bounds)
, mNodes(nodes)
{
}

void Quadtree::update()
{
    std::list<Node*> updatedNodes;
    updateNodes(updatedNodes);

    for(Node* node : updatedNodes)
        insertNode(node);

    updateTree();
}

void Quadtree::updateTree()
{
    if(mChildren.empty())
    {
        if(mQuadNodes.size() > MAX_NODES && mLevel < MAX_LEVELS)
        {
            split();

            for(Node* node : mQuadNodes)
            {
                unsigned char indices = getPartialIndices(node->entity->getBoundingRect());

                if(indices & (1 << 0))
                    mChildren[0].insertNode(node);
                if(indices & (1 << 1))
                    mChildren[1].insertNode(node);
                if(indices & (1 << 2))
                    mChildren[2].insertNode(node);
                if(indices & (1 << 3))
                    mChildren[3].insertNode(node);
            }
        }

    }
    else if(mQuadNodes.size() < MAX_NODES)
        merge();

    for(Quadtree& child : mChildren)
        child.updateTree();
}

void Quadtree::insertEntity(EntityNode* entity)
{
    for(Node& node : mNodes)
    {
        if(node.entity == entity)
            return;
    }

    Node node;
    node.entity = entity;

    auto insertionIt = mNodes.insert(mNodes.end(), node);
    insertNode(&(*insertionIt));
}

void Quadtree::updateNodes(std::list<Node*>& updatedNodes)
{
    removeWrecks();

    for(Node& node : mNodes)
        if(node.entity->isMoving())
        {
            sf::FloatRect entityRect = node.entity->getBoundingRect();
            std::list<Quadtree*> markedForErasion;
            auto iQuad = node.quads.begin();
            while(iQuad != node.quads.end())
            {
                if(!intersects(entityRect, (*iQuad)->getBoundingRect()) && !intersects((*iQuad)->getBoundingRect(), entityRect))
                    markedForErasion.push_back(*iQuad);

                updatedNodes.push_back(&node);

                iQuad++;
            }
            for(Quadtree* quad : markedForErasion)
                quad->eraseQuadNode(&node);
        }
}

void Quadtree::eraseNode(std::list<Node>::iterator iNode)
{
    std::list<Quadtree*> markedForRemoval;
    for(Quadtree* quad : iNode->quads)
        markedForRemoval.push_back(quad);

    Node* pNode = &(*iNode);
    for(Quadtree* quad : markedForRemoval)
        quad->eraseQuadNode(pNode);

    mNodes.erase(iNode);
}

sf::FloatRect Quadtree::getBoundingRect()
{
    return mBounds;
}

void Quadtree::eraseQuadNode(Node* node)
{
    auto iNode = std::find(mQuadNodes.begin(), mQuadNodes.end(), node);
    if(iNode != mQuadNodes.end())
    {
        mQuadNodes.erase(iNode);

        auto iQuad = std::find(node->quads.begin(), node->quads.end(), this);
        if(iQuad != node->quads.end())
            node->quads.erase(iQuad);
    }
}

bool Quadtree::isOutOfBounds(EntityNode* node)
{
    sf::FloatRect rect = node->getBoundingRect();

    if(rect.left < mBounds.left)
        return true;
    if(rect.top < mBounds.top)
        return true;
    if(rect.left + rect.width > mBounds.left + mBounds.width)
        return true;
    if(rect.top + rect.height > mBounds.top + mBounds.height)
        return true;

    return false;
}


void Quadtree::insertNode(Node* node)
{
    if(!mChildren.empty())
    {
        unsigned char indices = getPartialIndices(node->entity->getBoundingRect());

        if(indices & (1 << 0))
            mChildren[0].insertNode(node);
        if(indices & (1 << 1))
            mChildren[1].insertNode(node);
        if(indices & (1 << 2))
            mChildren[2].insertNode(node);
        if(indices & (1 << 3))
            mChildren[3].insertNode(node);
    }

    // Only insert the node if it doesn't already exist here.
    if(std::find(mQuadNodes.begin(), mQuadNodes.end(), node) == mQuadNodes.end())
    {
        mQuadNodes.push_back(node);
        node->quads.push_back(this);
    }
}

void Quadtree::removeWrecks()
{
    std::list<std::list<Node>::iterator> markedForRemoval;
    auto it = mNodes.begin();
    while(it != mNodes.end())
    {
        if(it->entity->isMarkedForRemoval() || it->quads.empty())
            markedForRemoval.push_back(it);

        it++;
    }

    for(auto it : markedForRemoval)
        eraseNode(it);
}

void Quadtree::split()
{
    float childWidth = mBounds.width / 2;
    float childHeight = mBounds.height / 2;

    float x = mBounds.left;
    float y = mBounds.top;

    mChildren.push_back(Quadtree(mLevel + 1, sf::FloatRect(x, y, childWidth, childHeight), mNodes)); //Top left
    mChildren.push_back(Quadtree(mLevel + 1, sf::FloatRect(x + childWidth, y, childWidth, childHeight), mNodes)); //Top right
    mChildren.push_back(Quadtree(mLevel + 1, sf::FloatRect(x, y + childHeight, childWidth, childHeight), mNodes)); //Bottom left
    mChildren.push_back(Quadtree(mLevel + 1, sf::FloatRect(x + childWidth, y + childHeight, childWidth, childHeight), mNodes)); //Bottom right
}


void Quadtree::merge()
{
    for(Quadtree& child : mChildren)
        child.clear();

    mChildren.clear();
}

void Quadtree::clear()
{
    for(Quadtree& child : mChildren)
        child.clear();

    std::list<Node*> nodes = mQuadNodes;
    for(Node* node : nodes)
        eraseQuadNode(node);

    mChildren.clear();
}


unsigned char Quadtree::getPartialIndices(sf::FloatRect nodeBounds)
{
    float midQuadX = mBounds.left + mBounds.width / 2;
    float midQuadY = mBounds.top + mBounds.height / 2;

    bool overlapsTopQuads = (nodeBounds.top < midQuadY);
    bool overlapsBottomQuads = (nodeBounds.top + nodeBounds.height > midQuadY);

    bool overlapsLeftQuads = (nodeBounds.left < midQuadX);
    bool overlapsRightQuads = (nodeBounds.left + nodeBounds.width > midQuadX);

    unsigned char partialIndices = 0;
    if(overlapsLeftQuads)
    {
        if(overlapsTopQuads)
            partialIndices |= (1 << 0);
        if(overlapsBottomQuads)
            partialIndices |= (1 << 2);
    }
    if(overlapsRightQuads)
    {
        if(overlapsTopQuads)
            partialIndices |= (1 << 1);
        if(overlapsBottomQuads)
            partialIndices |= (1 << 3);
    }

    return partialIndices;
}

std::list<std::pair<EntityNode*, EntityNode*>> Quadtree::getNearbyEntities()
{
    std::list<std::pair<EntityNode*, EntityNode*>> nearbyPairs;
    for(Node& node : mNodes)
    {
        std::list<Node*> nearbyNodes;
        for(Quadtree* quad : node.quads)
        {
            /*
             * If the quad is a bottom quad, i.e. it has
             * no children, insert all the nodes it contains
             * into nearbyNodes.
             */
            if(!quad->hasChildren())
            {
                std::list<Node*> quadNodes = quad->getQuadNodes();
                nearbyNodes.insert(nearbyNodes.end(), quadNodes.begin(), quadNodes.end());
            }
        }

        // Remove duplicate entries.
        nearbyNodes.unique();

        for(Node* nearbyNode : nearbyNodes)
        {
            // Push nearby pair into collission array if nearby node is NOT node.
            if(nearbyNode != &node)
            {
                // Order pair by pointer value.
                if(node.entity < nearbyNode->entity)
                    nearbyPairs.push_back(std::make_pair(node.entity, nearbyNode->entity));
                else
                    nearbyPairs.push_back(std::make_pair(nearbyNode->entity, node.entity));
            }
        }
    }

    /*
     * Remove duplicate entries. Since the pairs are ordered by pointer value
     * we are guaranteed to catch duplicates.
     */
    nearbyPairs.unique([](std::pair<EntityNode*, EntityNode*>& a, std::pair<EntityNode*, EntityNode*>& b){return a.first == b.first;});

    // Move the list out of here to avoid copying large-sized lists.
    return std::move(nearbyPairs);
}


bool Quadtree::hasChildren()
{
    return !mChildren.empty();
}

std::list<Quadtree::Node*> Quadtree::getQuadNodes()
{
    return mQuadNodes;
}


void Quadtree::getQuadtree(std::list<Quadtree*>& qtree)
{
    qtree.push_back(this);

    for(Quadtree& child : mChildren)
    {
        child.getQuadtree(qtree);
    }
}
