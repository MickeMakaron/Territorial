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
    // Do nothing if entity is marked for removal.
    if(entity->isMarkedForRemoval())
        return;

    // Do nothing if entity already exists in tree.
    for(Node& node : mNodes)
        if(node.entity == entity)
            return;

    Node node;
    node.entity = entity;

    auto insertionIt = mNodes.insert(mNodes.end(), node);
    insertNode(&(*insertionIt));
}

void Quadtree::eraseEntity(EntityNode* entity)
{
    for(auto it = mNodes.begin(); it != mNodes.end(); it++)
    {
        if(it->entity == entity)
        {
            eraseNode(it);
            return;
        }
    }
}

void Quadtree::updateNodes(std::list<Node*>& updatedNodes)
{
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



                iQuad++;
            }
            updatedNodes.push_back(&node);

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

sf::FloatRect Quadtree::getBoundingRect() const
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

void Quadtree::getBottomQuads(std::list<Quadtree*>& quads)
{
    if(mChildren.empty())
        quads.push_back(this);
    else
        for(Quadtree& child : mChildren)
            child.getBottomQuads(quads);
}

std::set<std::pair<EntityNode*, EntityNode*>> Quadtree::getNearbyEntities()
{
    std::set<std::pair<EntityNode*, EntityNode*>> nearbyPairs;

    std::list<Quadtree*> bottomQuads;
    getBottomQuads(bottomQuads);

    for(Quadtree* quad : bottomQuads)
    {
        std::list<Node*> nearbyNodes = quad->getQuadNodes();

        auto iNodeA = nearbyNodes.begin();
        auto iNodeB = iNodeA;
        while(iNodeA != nearbyNodes.end())
        {
            iNodeB++;
            auto it = iNodeB;
            while(it != nearbyNodes.end())
            {
                // Order pair by pointer value.
                if((*iNodeA)->entity < (*it)->entity)
                    nearbyPairs.insert(std::make_pair((*iNodeA)->entity, (*it)->entity));
                else
                    nearbyPairs.insert(std::make_pair((*it)->entity, (*iNodeA)->entity));

                it++;
            }

            iNodeA++;
        }
    }


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


void Quadtree::getQuadtree(std::list<const Quadtree*>& qtree) const
{
    qtree.push_back(this);

    for(const Quadtree& child : mChildren)
    {
        child.getQuadtree(qtree);
    }
}
