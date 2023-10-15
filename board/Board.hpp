//
// Created by mate on 2023.09.10.
//

#ifndef CHESS_BOARD_HPP
#define CHESS_BOARD_HPP
#pragma once
#include <SFML/Graphics.hpp>
#include "../texture/CustomTexture.hpp"
#include "../piece/Piece.hpp"

using PieceSharedPtr = std::shared_ptr<Piece>;

class Board: public CustomTexture {
private:
    PieceSharedPtr board[8][8];
public:
    explicit Board(std::string & path, sf::Vector2u screenSize, sf::Sprite &initSprite);
    void render(sf::RenderWindow& window) const;
    void loadPosition(std::array<std::array<char, 8>, 8> fen, std::array<sf::Sprite, 64>& pieceSprites);
    void movePiece(PieceSharedPtr & piece, int row, int col);
    PieceSharedPtr& getSquare(int row, int col) {
        return board[row][col];
    }
    void setSquare(int row, int col, PieceSharedPtr& piece) {
        board[row][col] = piece;
    }
    const PieceSharedPtr (&getBoard())[8][8] {
        return board;
    }
};

#endif //CHESS_BOARD_HPP
