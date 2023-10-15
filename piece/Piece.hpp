//
// Created by mate on 2023.09.17.
//

#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP
#pragma once
#include "../texture/CustomTexture.hpp"

const std::string PIECE_PATH = "../static/img/pieces/";

struct PiecePosition {
    int row = 0;
    int col = 0;
};

class Piece : public CustomTexture {
private:
    PiecePosition position;
    char name;
public:
    explicit Piece(sf::Sprite &initSprite, char name);
    PiecePosition getPosition() const {
        return position;
    }
    void setPosition(int row, int col) {
        position.row = std::min(std::max(row, 0), 7);
        position.col = std::min(std::max(col, 0), 7);
    }
    void setPosition(PiecePosition piecePosition) {
        position = piecePosition;
    }
    void movePosition(int row, int col) {
        setPosition(position.row + row, position.col + col);
    }
};


#endif //CHESS_PIECE_HPP
