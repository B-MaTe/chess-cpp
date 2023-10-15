//
// Created by mate on 2023.09.10.
//

#ifndef CHESS_CUSTOM_TEXTURE_H
#define CHESS_CUSTOM_TEXTURE_H
#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

class CustomTexture {
public:
    explicit CustomTexture(const std::string& path, sf::Sprite &initSprite);

    void loadFromFile(const std::string& path);

    sf::Texture getTexture() const;

    sf::Sprite & getSprite() const;

private:
    sf::Texture texture;
    sf::Sprite & sprite;
};

#endif //CHESS_CUSTOM_TEXTURE_H
