//
// Created by mate on 2023.09.10.
//

#include "CustomTexture.hpp"

CustomTexture::CustomTexture(const std::string& path, sf::Sprite &initSprite): sprite(initSprite) {
    loadFromFile(path);

}

void CustomTexture::loadFromFile(const std::string& path) {
    try {
        if (texture.loadFromFile(path)) {
            getSprite().setTexture(texture);
        }
    } catch (const std::exception& e) {
        std::cerr << "Issue while loading image." << std::endl;
        std::cerr << e.what() << std::endl;
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
