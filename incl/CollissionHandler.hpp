#ifndef ANTGAME_COLLISSIONHANDLER_HPP
#define ANTGAME_COLLISSIONHANDLER_HPP

////////////////////////////////////////////////
// STD - C++ Standard Library
#include <list>
////////////////////////////////////////////////

#include "CollissionFinder.hpp"


class CollissionHandler
{
    public:
        CollissionHandler(sf::FloatRect area);

        void    update();
        void    insert(EntityNode* entity);

    private:
        typedef CollissionFinder::CollissionData CollissionData;
        CollissionFinder    mCollissionFinder;
};

#endif //ANTGAME_COLLISSIONHANDLER_HPP
