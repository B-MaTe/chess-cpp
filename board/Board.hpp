//
// Created by mate on 2023.09.10.
//

#ifndef CHESS_BOARD_HPP
#define CHESS_BOARD_HPP
#pragma once
#include <SFML/Graphics.hpp>
#include "../texture/CustomTexture.hpp"
#include "../piece/Piece.hpp"
#include "../prompt/PiecePromotionPrompt.hpp"
#include <cmath>

using PieceSharedPtr = std::shared_ptr<Piece>;

class Board: public CustomTexture {
private:
    std::vector<std::shared_ptr<sf::Shape>> availablePositionBackgrounds;
    PieceSharedPtr board[8][8];
    PieceSharedPtr findPieceSharedPtrByPosition(sf::Vector2f position);
    bool active = true;
    bool justPromoted = false;
    PiecePromotionPrompt* piecePromotionPrompt;
    std::array<sf::Sprite*, 8> promotionPieceSprites;
    std::array<sf::Texture, 8> promotionPieceTextures;
public:
    explicit Board(std::string & path, sf::Vector2u screenSize, sf::Sprite &initSprite);
    ~Board();
    void static drawBoard(PieceSharedPtr (&boardToPrint)[8][8]);
    void render(sf::RenderWindow& window, sf::Event& event);
    void loadPosition(std::array<std::array<char, 8>, 8> fen, std::array<sf::Sprite, 64>& pieceSprites, bool whiteAtBottom, sf::RenderWindow &window);
    bool movePiece(PieceSharedPtr & piece, int row, int col, bool countsAsMove, bool whiteAtBottom, sf::RenderWindow &window);
    bool movePiece(PieceSharedPtr & piece, PiecePosition position, bool countsAsMove, bool whiteAtBottom, sf::RenderWindow &window) {
        return movePiece(piece, position.row, position.col, countsAsMove, whiteAtBottom, window);
    }
    PieceSharedPtr& getSquare(PiecePosition position) {
        return board[position.row][position.col];
    }
    bool pawnPromotionMove(PieceSharedPtr & piece, bool whiteAtBottom);
    PiecePosition normalizePosition(int x, int y);
    bool normalizePositionAndMovePiece(sf::Vector2f newPosition, sf::Vector2i mousePosition, bool whiteAtBottom, sf::RenderWindow &window);
    PieceSharedPtr (&getBoard())[8][8] {
        return board;
    }
    void setPieceSpriteProperties(sf::Sprite& pieceSprite, PieceSharedPtr & piece);
    void colorSquares(const std::vector<PiecePosition>& positions);
    void promotePawn(PieceSharedPtr & piece);
    void resetAvailablePositionBackgrounds() {
        availablePositionBackgrounds.clear();
    }
    void setActive(bool _active) {
        active = _active;
    }
    bool isActive() const {
        return active;
    }
    void setJustPromoted(bool isJustPromoted) {
        justPromoted = isJustPromoted;
    }
    bool isJustPromoted() const {
        return justPromoted;
    }
};

#endif //CHESS_BOARD_HPP
