//
// Created by mate on 2023.09.10.
//

#include "CustomTexture.hpp"

CustomTexture::CustomTexture(const std::string& path, sf::Sprite &initSprite): sprite(initSprite) {
    loadFromFile(path);

}

void CustomTexture::loadFromFile(const std::string& path) {
    if (texture.loadFromFile(path)) {
        getSprite().setTexture(texture);
    } else {
        exit(EXIT_FAILURE);
    }
}

sf::Texture CustomTexture::getTexture() const {
    return texture;
}

sf::Sprite & CustomTexture::getSprite() const
{
    return sprite;
}
