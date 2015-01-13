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

#include "Team.hpp"

Team::Team(unsigned int id)
: mId(id)
, mAllies(id)
, mHostiles(0)
{

}

const unsigned int& Team::getAllies() const
{
    return mAllies;
}

const unsigned int& Team::getHostiles() const
{
    return mHostiles;
}

const unsigned int& Team::getId() const
{
    return mId;
}

void Team::addAlly(unsigned int teamId)
{
    mAllies |= teamId;
}

void Team::addNeutral(unsigned int teamId)
{
    // Set team bit to zero in both mAllies and mHostiles.
    mAllies &= ~teamId;
    mHostiles &= ~teamId;
}

void Team::addHostile(unsigned int teamId)
{
    mHostiles |= teamId;
}


bool Team::isAllied(unsigned int teamId)
{
    return mAllies & teamId;
}

bool Team::isNeutral(unsigned int teamId)
{
    return !((mAllies | mHostiles) & teamId);
}

bool Team::isHostile(unsigned int teamId)
{
    return mHostiles & teamId;
}
