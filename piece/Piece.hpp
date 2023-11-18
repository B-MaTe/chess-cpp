//
// Created by mate on 2023.09.17.
//

#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP
#pragma once
#include "../texture/CustomTexture.hpp"
#include "../globals.hpp"

const std::string PIECE_PATH = "../static/img/pieces/";


class Piece : public CustomTexture {
private:
    PiecePosition position;
    char name;
    bool currentlyMoving = false;
    bool moved = false;
    bool white;
public:
    static std::string getPath(char _name);
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
    void setName(char _name) {
        name = _name;
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
