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
        void    handleCollissions(std::list<CollissionFinder::CollissionData> collissions);

    private:
        typedef CollissionFinder::CollissionData CollissionData;
};

#endif //ANTGAME_COLLISSIONHANDLER_HPP
