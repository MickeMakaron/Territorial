////////////////////////////////////////////////
// Super Fast Media Library (SFML)
#include "SFML/Graphics/RenderTarget.hpp"
////////////////////////////////////////////////

#include "CursorNode.hpp"
#include "EntityNode.hpp"
#include "Utility.hpp"

CursorNode::CursorNode(sf::RenderTarget& target)
: mTarget(target)
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
        case sf::Event::MouseMoved:
            //setPosition(mTarget.mapPixelToCoords(sf::Mouse::getPosition()));
            setPosition(pix2coords(event.mouseMove.x, event.mouseMove.y));
            //setPosition(event.mouseMove.x, event.mouseMove.y);
            break;
        case sf::Event::MouseLeft:
            setInvisible();
            break;
        case sf::Event::MouseEntered:
            setVisible();
            break;
        case sf::Event::MouseButtonReleased:
            if(event.mouseButton.button == sf::Mouse::Button::Left)
                activate(event.mouseButton);
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
    {
        mActivation->setPosition(pix2coords(event.x, event.y));
    }
}

sf::Vector2f CursorNode::pix2coords(const int& x, const int& y) const
{
    return pix2coords(sf::Vector2i(x, y));
}

sf::Vector2f CursorNode::pix2coords(const sf::Vector2i& pixel) const
{
    return mTarget.mapPixelToCoords(pixel);
}

void CursorNode::activate(const sf::Event::MouseButtonEvent& event)
{
    if(mSelection)
    {
        if(intersects(pix2coords(event.x, event.y), mSelection->getBoundingRect()))
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
