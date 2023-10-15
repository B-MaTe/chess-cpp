//
// Created by mate on 2023.09.17.
//

#include "Piece.hpp"

std::string static getPath(char _name) {

    return PIECE_PATH + std::string(1, _name) + ".png";
}

Piece::Piece(sf::Sprite &initSprite, char _name) : CustomTexture(getPath(_name), initSprite)
{
    position = {0,0};
    name = _name;
}




