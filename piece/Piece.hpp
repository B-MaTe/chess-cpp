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

    bool operator==(const PiecePosition& other) const {
        return (row == other.row) && (col == other.col);
    }

    bool operator!=(const PiecePosition& other) const {
        return !(*this == other);
    }
};

class Piece : public CustomTexture {
private:
    PiecePosition position;
    char name;
    bool currentlyMoving = false;
    bool moved = false;
    bool white;
public:
    explicit Piece(sf::Sprite &initSprite, char name);
    bool isAtStartingSquare(bool whiteAtBottom);
    PiecePosition getPosition() const {
        return position;
    }
    void setPosition(PiecePosition piecePosition) {
        position = piecePosition;
    }
    char getName() const {
        return name;
    }
    bool isCurrentlyMoving() const {
        return currentlyMoving;
    }
    void setCurrentlyMoving(bool isCurrentlyMoving) {
        currentlyMoving = isCurrentlyMoving;
    }
    bool isMoved() const
    {
        return moved;
    }
    void setMoved(bool _moved)
    {
        moved = _moved;
    }
    bool isWhite() const {
        return white;
    }
};


#endif //CHESS_PIECE_HPP
