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

#include "SceneNode.hpp"

#include "Command.hpp"
#include "Utility.hpp"

////////////////////////////////////////////////
// C++ Standard Library
#include <algorithm>
#include <cassert>
#include <cmath>
////////////////////////////////////////////////

////////////////////////////////////////////////
// For debugging
#include <string>
#include <sstream>
#include <iostream>
////////////////////////////////////////////////

////////////////////////////////////////////////
// Super Fast Media Library (SFML)
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////


SceneNode::SceneNode(Category::Type category)
: mChildren()
, mParent(nullptr)
, mDefaultCategory(category)
{

}

void SceneNode::attachChild(SceneNode::Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&] (Ptr& p) { return p.get() == &node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
    updateCurrent(dt, commands);
    updateCurrent();
	updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time, CommandQueue&)
{
	// Do nothing by default
}

void SceneNode::updateCurrent()
{
	// Do nothing by default.
}

void SceneNode::destroy()
{
    destroyCurrent();
    destroyChildren();
}

void SceneNode::destroyCurrent()
{
    // Do nothing by default.
}

void SceneNode::destroyChildren()
{
    for(Ptr& child : mChildren)
        child->destroy();
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for(Ptr& child : mChildren)
    {
        if(child->isMarkedForRemoval())
            child->destroy();

        child->update(dt, commands);
    }

    // Remove children marked for removal.
    auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
    mChildren.erase(wreckfieldBegin, mChildren.end());
}



void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply transform of current node
	states.transform *= getTransform();

    drawCurrent(target, states);

    // Draw children with changed transform
	drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
	// Do nothing by default.
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const Ptr& child : mChildren)
            child->draw(target, states);
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates) const
{
	sf::FloatRect rect = getBoundingRect();

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Blue);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
		transform = node->getTransform() * transform;

	return transform;
}

bool SceneNode::isMarkedForRemoval() const
{
    // Return false by default.
    return false;
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	// Command current node, if category matches
	if (command.category & getCategory())
		command.action(*this, dt);

	// Command children
	for(Ptr& child : mChildren)
		child->onCommand(command, dt);
}

unsigned int SceneNode::getCategory() const
{
	return mDefaultCategory;
}


sf::FloatRect SceneNode::getBoundingRect() const
{
    // By default, a node has no bounds
	return sf::FloatRect();
}


SceneNode* SceneNode::getParent() const
{
    return mParent;
}
