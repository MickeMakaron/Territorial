#ifndef ANTGAME_WORLD_HPP
#define ANTGAME_WORLD_HPP

////////////////////////////////////////////////
// C++ Standard Library
#include <list>
#include <memory>
////////////////////////////////////////////////


////////////////////////////////////////////////
// Super Fast Media Library (SFML)
#include "SFML/System/Time.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
////////////////////////////////////////////////

#include "EntityNode.hpp"
#include "ResourceHolder.hpp"
#include "CommandQueue.hpp"
#include "CursorNode.hpp"

namespace sf
{
    class RenderWindow;
    class RenderTarget;
}

class World
{
    public:
        World(sf::RenderWindow& window);
        void draw();
        void update(sf::Time dt);
        void handleEvent(const sf::Event& event);


    private:
        void buildWorld();


    private:
        SceneNode mEntitiesGraph;
        sf::Sprite mBackground;
        sf::RenderWindow& mWindow;
        sf::RenderTarget& mTarget;
        CursorNode mCursorNode;
        CommandQueue mCommandQueue;
        ResourceHolder<sf::Texture, int> mTextures;
};

#endif // ANTGAME_HPP
