#ifndef ANTGAME_ANTGAME_HPP
#define ANTGAME_ANTGAME_HPP

////////////////////////////////////////////////
// Super Fast Media Library (SFML)
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
////////////////////////////////////////////////

#include "World.hpp"

class StateMachine
{
    public:
        bool isEmpty()
        {
            return false;
        }

        void handleEvent(const sf::Event& event)
        {

        }
};

class AntGame
{

    public:
        AntGame(unsigned int sizeX, unsigned int sizeY);

        void processInput();
        void update(sf::Time dt);
        void render();
        void run();


    private:
        sf::RenderWindow mWindow; ///< SFML window class.
        StateMachine mStateMachine; ///< State machine for menus and other states.
        const sf::Time TIME_PER_FRAME = sf::seconds(1.f/60.f); ///< Inverse FPS
        World mWorld; ///< World state
};


#endif // ANTGAME_ANTGAME_HPP
