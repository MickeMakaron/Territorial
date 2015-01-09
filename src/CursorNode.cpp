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
, mSelection(nullptr)
, mActivation(nullptr)
{

    mSelectCommand.category = Category::Entity;
    mSelectCommand.action = derivedAction<EntityNode>([this](EntityNode& node, sf::Time)
    {
        if(intersects(getWorldPosition(), node.getBoundingRect()) && !node.isMarkedForRemoval())
            mSelection = &node;
    });
}

void CursorNode::handleEvent(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::MouseLeft:
            setInvisible();
            break;
        case sf::Event::MouseEntered:
            setVisible();
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
    if(mActivation)
        mActivation->setPosition(pix2coords(event.x, event.y));
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
    if(mSelection)
    {
        if(intersects(getWorldPosition(), mSelection->getBoundingRect()))
            mActivation = mSelection;
        else
        {
            mSelection = nullptr;
            mActivation = nullptr;
        }
    }
    else
        mActivation = nullptr;
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
    if(!mSelection)
        commands.push(mSelectCommand);
    else if(mSelection->isMarkedForRemoval())
        mSelection = nullptr;
    else if(!intersects(getWorldPosition(), mSelection->getBoundingRect()))
    {
        mSelection = nullptr;
        commands.push(mSelectCommand);
    }
}

void CursorNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    refreshSelection(commands);
    setPosition(pix2coords(sf::Mouse::getPosition(mWindow)));

    if(mActivation && mActivation->isMarkedForRemoval())
        mActivation = nullptr;

    mLastPos = getWorldPosition();
}

bool CursorNode::hasMoved() const
{
    return getWorldPosition() != mLastPos;
}

void CursorNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);

    if(mActivation)
        drawBoundingRect(target, states);
    else if(mSelection)
        mSelection->drawBoundingRect(target, states);
}
