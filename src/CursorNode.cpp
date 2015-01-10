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


////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
////////////////////////////////////////////////

#include "CursorNode.hpp"
#include "EntityNode.hpp"
#include "Utility.hpp"

CursorNode::CursorNode(sf::RenderWindow& window, sf::RenderTarget& target)
: mWindow(window)
, mTarget(target)
, mHasSelectionBox(false)
, mSelectionBoxNeedsUpdate(false)
{
	mSelectionBox.setFillColor(sf::Color::Transparent);
	mSelectionBox.setOutlineColor(sf::Color::Blue);
    mSelectionBox.setOutlineThickness(1.f);

    mSelectCommand.category = Category::Entity;
    mSelectCommand.action = derivedAction<EntityNode>([this](EntityNode& node)
    {
        if(!node.isMarkedForRemoval() && intersects(getWorldPosition(), node.getBoundingRect()))
            pushSelection(&node);
    });

    mSelectBoxCommand.category = Category::Entity;
    mSelectBoxCommand.action = derivedAction<EntityNode>([this](EntityNode& node)
    {
        if(!node.isMarkedForRemoval()
           && (intersects(node.getBoundingRect(), sf::FloatRect(mSelectionBox.getPosition(), mSelectionBox.getSize()))
           || intersects(sf::FloatRect(mSelectionBox.getPosition(), mSelectionBox.getSize()), node.getBoundingRect())))
            pushSelection(&node);
    });
}

void CursorNode::pushSelection(EntityNode* node)
{
    sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Blue);
    shape.setOutlineThickness(1.f);

	mSelections.push_back(Highlight(node, shape));
}

void CursorNode::pushActivation(EntityNode* node)
{
    sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Blue);
    shape.setOutlineThickness(3.f);

	mActivations.push_back(Highlight(node, shape));
}


void CursorNode::updateSelectionBox(sf::Vector2f mousePos)
{
    if(mHasSelectionBox)
    {
        sf::Vector2f dVec = mousePos - mSelectionBoxStartPos;
        mSelectionBox.setPosition(mSelectionBoxStartPos);
        mSelectionBox.setSize(dVec);

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
        sf::Vector2f dVec = mousePos - mMouseDownPos;
        float dSqrd = dVec.x * dVec.x + dVec.y * dVec.y;

        if(dSqrd > 100)
        {
            mSelectionBoxStartPos = mousePos;
            mSelectionBox.setPosition(mousePos);
            mSelectionBox.setSize(sf::Vector2f(0, 0));
            mHasSelectionBox = true;
        }
    }
}

void CursorNode::handleEvent(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::MouseMoved:
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                mSelectionBoxNeedsUpdate = true;
            break;
        case sf::Event::MouseLeft:
            setInvisible();
            break;
        case sf::Event::MouseEntered:
            setVisible();
            break;
        case sf::Event::MouseButtonPressed:
            mMouseDownPos = pix2coords(event.mouseButton.x, event.mouseButton.y);
            break;
        case sf::Event::MouseButtonReleased:
            if(event.mouseButton.button == sf::Mouse::Button::Left)
                activate();
            else if(event.mouseButton.button == sf::Mouse::Button::Right)
                onRightMouseRelease(event.mouseButton);
            break;
        default:
            break;
    }
}

void CursorNode::onRightMouseRelease(const sf::Event::MouseButtonEvent& event)
{
    for(const Highlight& highlight: mActivations)
        highlight.node->setDestination(pix2coords(event.x, event.y));
}

sf::Vector2f CursorNode::pix2coords(const int& x, const int& y) const
{
    return pix2coords(sf::Vector2i(x, y));
}

sf::Vector2f CursorNode::pix2coords(const sf::Vector2i& pixel) const
{
    return mTarget.mapPixelToCoords(pixel);
}

void CursorNode::activate()
{
    if(mHasSelectionBox)
    {
        mActivations.clear();

        for(const Highlight& selection : mSelections)
            pushActivation(selection.node);

        mHasSelectionBox = false;
    }
    else if(mSelections.size() > 0)
    {
        EntityNode* pSelection = mSelections[0].node;
        sf::FloatRect selectionRect = pSelection->getBoundingRect();
        if(intersects(mMouseDownPos, selectionRect) && intersects(mMouseDownPos, selectionRect))
        {
            mActivations.clear();
            pushActivation(pSelection);
        }
        else
        {
            mActivations.clear();
            mSelections.clear();
        }
    }
    else
        mActivations.clear();
}

sf::FloatRect CursorNode::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void CursorNode::setTexture(const sf::Texture& texture)
{
    mSprite.setTexture(texture, true);
    mTextureRect = mSprite.getTextureRect();

    mSprite.setTextureRect(mTextureRect);
}

void CursorNode::setVisible()
{
    mSprite.setTextureRect(mTextureRect);
}

void CursorNode::setInvisible()
{
    mSprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
}

void CursorNode::refreshSelection(CommandQueue& commands)
{
    if(mHasSelectionBox)
        commands.push(mSelectBoxCommand);
    else
        commands.push(mSelectCommand);

    auto wreckfieldBegin = std::remove_if(mSelections.begin(), mSelections.end(), [](const Highlight& highlight){return highlight.node->isMarkedForRemoval();});
    mSelections.erase(wreckfieldBegin, mSelections.end());


    if(mHasSelectionBox)
    {
        sf::FloatRect boxRect(mSelectionBox.getPosition(), mSelectionBox.getSize());
        wreckfieldBegin = std::remove_if(mSelections.begin(), mSelections.end(), [boxRect](const Highlight& highlight){return !intersects(highlight.node->getBoundingRect(), boxRect);});
        mSelections.erase(wreckfieldBegin, mSelections.end());
    }
    else
    {
        sf::Vector2f worldPos = getWorldPosition();
        wreckfieldBegin = std::remove_if(mSelections.begin(), mSelections.end(), [worldPos](const Highlight& highlight){return !intersects(worldPos, highlight.node->getBoundingRect());});
        mSelections.erase(wreckfieldBegin, mSelections.end());
    }


    for(Highlight& highlight : mSelections)
        updateOutline(highlight.node, highlight.outline);
}

void CursorNode::updateOutline(const EntityNode* node, sf::RectangleShape& outline)
{
    sf::FloatRect rect = node->getBoundingRect();
    outline.setPosition(rect.left, rect.top);
    outline.setSize(sf::Vector2f(rect.width, rect.height));
}

void CursorNode::updateCurrent(CommandQueue& commands)
{
    if(mSelectionBoxNeedsUpdate)
    {
        updateSelectionBox(pix2coords(sf::Mouse::getPosition(mWindow)));
        mSelectionBoxNeedsUpdate = false;
    }


    refreshSelection(commands);
    setPosition(pix2coords(sf::Mouse::getPosition(mWindow)));

    auto wreckfieldBegin = std::remove_if(mActivations.begin(), mActivations.end(), [](const Highlight& highlight){return highlight.node->isMarkedForRemoval();});
    mActivations.erase(wreckfieldBegin, mActivations.end());


    for(Highlight& highlight : mActivations)
        updateOutline(highlight.node, highlight.outline);

    mLastPos = getWorldPosition();
}

bool CursorNode::hasMoved() const
{
    return getWorldPosition() != mLastPos;
}

void CursorNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);



    for(const Highlight& highlight : mActivations)
        target.draw(highlight.outline);

    for(const Highlight& highlight : mSelections)
        target.draw(highlight.outline);

    if(mHasSelectionBox)
        target.draw(mSelectionBox);
}
