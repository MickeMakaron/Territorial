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

#ifndef ANTGAME_MAP_HPP
#define ANTGAME_MAP_HPP

////////////////////////////////////////////////
// SFML - Simple and Fast Media Library
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
////////////////////////////////////////////////

#include "TerrainCollissionNode.hpp"

class Map : public sf::Drawable
{
    public:
        typedef std::unique_ptr<TerrainCollissionNode> NodePtr;

        Map(const std::string& filePath);

        void load(const std::string& filePath);
        sf::FloatRect getBounds() const;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        const std::list<NodePtr>& getImpassableTerrain() const;

    private:
        sf::Texture         mTexture;
        sf::RectangleShape  mDrawShape;

        std::list<NodePtr> mImpassableNodes;

};

#endif //ANTGAME_MAP_HPP
