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

#ifndef ANTGAME_RESOURCEHOLDER_HPP
#define ANTGAME_RESOURCEHOLDER_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>






template <typename Resource, typename Identifier>
class ResourceHolder
{
    public:
        void                load(Identifier id,const std::string& filename);
        void                load(Identifier id, const Resource& resource);


        template <typename Parameter>
        void                load(Identifier id, const std::string& filename, const Parameter& secondParam);

        Resource&           get(Identifier id);
        const Resource&     get(Identifier id) const;

        bool                isLoaded(Identifier id);

        void  release(Identifier id);

    private:
        typedef std::pair<std::shared_ptr<Resource>, int> Pair;
        void insertResource(Identifier id, std::shared_ptr<Resource> resource);

    private:
        std::map<Identifier, Pair > mResourceMap;
};

#include "ResourceHolder.inl"

#endif // ANTGAME_RESOURCEHOLDER_HPP
