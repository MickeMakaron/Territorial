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

#include "EntitySelector.hpp"
#include "EntityNode.hpp"
#include "Utility.hpp"
#include "CommandQueue.hpp"


#include <algorithm>

#include "SFML/Graphics/RenderTarget.hpp"


EntitySelector::EntitySelector()
: mHasSelectionBox(false)
, mIsSelecting(false)
{
	mSelectionBox.setFillColor(sf::Color::Transparent);
	mSelectionBox.setOutlineColor(sf::Color::Blue);
    mSelectionBox.setOutlineThickness(1.f);

    mSelectCommand.category = Category::Entity;
    mSelectCommand.action = derivedAction<EntityNode>([this](EntityNode& node)
    {
        if(!node.isMarkedForRemoval() && mSelections.size() < 1 && intersects(mPos, node.getBoundingRect()))
        {
            for(const Highlight& selection : mSelections)
                if(selection.node == &node)
                    return;

            pushSelection(&node);
        }

    });



    mSelectBoxCommand.category = Category::PlayerEntity;
    mSelectBoxCommand.action = derivedAction<EntityNode>([this](EntityNode& node)
    {
        if(!node.isMarkedForRemoval()
           && (intersects(node.getBoundingRect(), sf::FloatRect(mSelectionBox.getPosition(), mSelectionBox.getSize()))
           || intersects(sf::FloatRect(mSelectionBox.getPosition(), mSelectionBox.getSize()), node.getBoundingRect())))
        {
            for(const Highlight& selection : mSelections)
                if(selection.node == &node)
                    return;

            pushSelection(&node);
        }
    });
}


void EntitySelector::setPosition(sf::Vector2f pos)
{
    mPos = pos;
}

void EntitySelector::startSelection(sf::Vector2f pos)
{
    mIsSelecting = true;
    mSelectionStart = pos;
}

void EntitySelector::endSelection(sf::Vector2f pos)
{
    activate();
    mIsSelecting = false;
    mHasSelectionBox = false;
}

void EntitySelector::pushSelection(EntityNode* node)
{
    sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Blue);
    shape.setOutlineThickness(1.f);

	mSelections.push_back(Highlight(node, shape));
}

void EntitySelector::pushActivation(EntityNode* node)
{
    sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Blue);
    shape.setOutlineThickness(3.f);

	mActivations.push_back(Highlight(node, shape));
}

void EntitySelector::interact(sf::Vector2f pos)
{
    if(mSelections.size() > 0)
    {
        for(Highlight& activation : mActivations)
            if(activation.node->getCategory() & Category::PlayerEntity)
                for(Highlight& selection : mSelections)
                    activation.node->interact(selection.node);
    }
    else
    {
        for(Highlight& activation : mActivations)
            if(activation.node->getCategory() & Category::PlayerEntity)
                activation.node->goTo(pos);
    }
}

void EntitySelector::activate()
{
    mActivations.clear();

    // If user is dragging a selection box, activate all selections. Else just one.
    if(mHasSelectionBox)
        for(const Highlight& selection : mSelections)
            pushActivation(selection.node);
    else if(mSelections.size() > 0)
        pushActivation(mSelections.begin()->node);

    mSelections.clear();
}

void EntitySelector::refreshSelections(CommandQueue& commands)
{
    if(mHasSelectionBox)
        commands.push(mSelectBoxCommand);
    else
        commands.push(mSelectCommand);

    if(mHasSelectionBox)
    {
        sf::FloatRect boxRect(mSelectionBox.getPosition(), mSelectionBox.getSize());
        auto wreckfieldBegin = std::remove_if(mSelections.begin(), mSelections.end(), [boxRect](const Highlight& highlight){return !intersects(highlight.node->getBoundingRect(), boxRect);});
        mSelections.erase(wreckfieldBegin, mSelections.end());
    }
    else
    {
        auto wreckfieldBegin = std::remove_if(mSelections.begin(), mSelections.end(), [this](const Highlight& highlight){return !intersects(mPos, highlight.node->getBoundingRect());});
        mSelections.erase(wreckfieldBegin, mSelections.end());
    }


    for(Highlight& highlight : mSelections)
        updateOutline(highlight.node, highlight.outline);
}

void EntitySelector::updateOutline(const EntityNode* node, sf::RectangleShape& outline)
{
    sf::FloatRect rect = node->getBoundingRect();
    outline.setPosition(rect.left, rect.top);
    outline.setSize(sf::Vector2f(rect.width, rect.height));
}

bool EntitySelector::isSelecting() const
{
    return mIsSelecting;
}

void EntitySelector::updateSelection(sf::Vector2f mousePos)
{
    if(mHasSelectionBox)
    {
        sf::Vector2f dVec = mousePos - mSelectionStart;
        mSelectionBox.setPosition(mSelectionStart);
        mSelectionBox.setSize(dVec);

        /*
         * If user is creating a selection box that is inverted,
         * i.e. dragging leftwards/upwards, a negative width/height
         * is created. If such a thing has occured, readjust the box
         * so it has a positive size.
         */
        if(dVec.x < 0 || dVec.y < 0)
        {
            sf::Vector2f size = mSelectionBox.getSize();
            sf::Vector2f pos = mSelectionBox.getPosition();

            if(dVec.x < 0)
            {
                pos.x = mousePos.x;
                size.x = -dVec.x;
            }

            if(dVec.y < 0)
            {
                pos.y = mousePos.y;
                size.y = -dVec.y;
            }

            mSelectionBox.setPosition(pos);
            mSelectionBox.setSize(size);
        }
    }
    else
    {
        sf::Vector2f dVec = mousePos - mSelectionStart;
        float dSqrd = dVec.x * dVec.x + dVec.y * dVec.y;

        if(dSqrd > 100)
        {
            mSelectionBox.setPosition(mousePos);
            mSelectionBox.setSize(sf::Vector2f(0, 0));
            mHasSelectionBox = true;
        }
    }
}


void EntitySelector::removeWrecks()
{
    auto wreckfieldBegin = std::remove_if(mActivations.begin(), mActivations.end(), [](const Highlight& highlight){return highlight.node->isMarkedForRemoval();});
    mActivations.erase(wreckfieldBegin, mActivations.end());

    wreckfieldBegin = std::remove_if(mSelections.begin(), mSelections.end(), [](const Highlight& highlight){return highlight.node->isMarkedForRemoval();});
    mSelections.erase(wreckfieldBegin, mSelections.end());
}

void EntitySelector::update(CommandQueue& commands)
{
    removeWrecks();
    refreshSelections(commands);


    for(Highlight& highlight : mActivations)
        updateOutline(highlight.node, highlight.outline);
}

void EntitySelector::draw(sf::RenderTarget& target) const
{
    for(const Highlight& highlight : mActivations)
        target.draw(highlight.outline);

    for(const Highlight& highlight : mSelections)
        target.draw(highlight.outline);

    if(mHasSelectionBox)
        target.draw(mSelectionBox);
}
