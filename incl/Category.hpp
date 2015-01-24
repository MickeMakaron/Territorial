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

#ifndef GAME_CATEGORY_HPP
#define GAME_CATEGORY_HPP


// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None				= 0,
		Highlight           = 1 << 0,
		PlayerEntity        = 1 << 1,
		ComputerEntity      = 1 << 2,
		OtherPlayerEntity   = 1 << 3,
		HostileEntity       = 1 << 4,
		AlliedEntity        = 1 << 5,
		NeutralEntity       = 1 << 6,
		Pathfinder         = 1 << 7,

        Entity = PlayerEntity | ComputerEntity | OtherPlayerEntity,
	};
}

#endif // GAME_CATEGORY_HPP
