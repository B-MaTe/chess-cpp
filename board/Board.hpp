//
// Created by mate on 2023.09.10.
//

#ifndef CHESS_BOARD_HPP
#define CHESS_BOARD_HPP
#pragma once
#include <SFML/Graphics.hpp>
#include "../texture/CustomTexture.hpp"
#include "../piece/Piece.hpp"
#include <cmath>

using PieceSharedPtr = std::shared_ptr<Piece>;

class Board: public CustomTexture {
private:
    std::vector<std::shared_ptr<sf::Shape>> availablePositionBackgrounds;
    PieceSharedPtr board[8][8];
    PieceSharedPtr findPieceSharedPtrByPosition(sf::Vector2f position);
public:
    explicit Board(std::string & path, sf::Vector2u screenSize, sf::Sprite &initSprite);
    void static drawBoard(PieceSharedPtr (&boardToPrint)[8][8]);
    void render(sf::RenderWindow& window) const;
    void loadPosition(std::array<std::array<char, 8>, 8> fen, std::array<sf::Sprite, 64>& pieceSprites);
    bool movePiece(PieceSharedPtr & piece, int row, int col);
    bool movePiece(PieceSharedPtr & piece, PiecePosition position) {
        return movePiece(piece, position.row, position.col);
    }
    PieceSharedPtr& getSquare(PiecePosition position) {
        return board[position.row][position.col];
    }
    PiecePosition normalizePosition(int x, int y);
    bool normalizePositionAndMovePiece(sf::Vector2f newPosition, sf::Vector2f initialPosition, sf::Vector2i mousePosition);
    PieceSharedPtr (&getBoard())[8][8] {
        return board;
    }
    void colorSquares(const std::vector<PiecePosition>& positions);
    void resetAvailablePositionBackgrounds() {
        availablePositionBackgrounds.clear();
    }
};

#endif //CHESS_BOARD_HPP
