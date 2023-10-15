//
// Created by mate on 2023.09.10.
//

#include "Board.hpp"

Board::Board(std::string& path, sf::Vector2u screenSize, sf::Sprite &initSprite) : CustomTexture(path, initSprite)
{
    const int BOARD_SIZE = 4000;
    float scale = (std::min(screenSize.x, screenSize.y) - 10.f) / BOARD_SIZE;
    getSprite().setScale(scale, scale);
    float spriteCenterX = (getSprite().getLocalBounds().width * scale) / 2.0;
    float spriteCenterY = (getSprite().getLocalBounds().height * scale) / 2.0;
    getSprite().setPosition((screenSize.x / 2) - spriteCenterX, (screenSize.y / 2) - spriteCenterY);

    for (auto & i : board) {
        for (int j = 0; j < 8; ++j) {
            i[j] = PieceSharedPtr(nullptr);
        }
    }
}

void Board::render(sf::RenderWindow &window) const
{
    window.draw(getSprite());
    for (const auto & i : board) {
        for (int j = 0; j < 8; ++j) {
            if (i[j] != PieceSharedPtr(nullptr)) {
                window.draw(i[j]->getSprite());
            }
        }
    }
}

void Board::loadPosition(std::array<std::array<char, 8>, 8> fen, std::array<sf::Sprite, 64>& pieceSprites)
{
    for (int i = 0; i < 64; i++)
    {
        pieceSprites[i] = sf::Sprite();
    }

    float pieceSize = (getSprite().getGlobalBounds().width / 8) * 0.68;
    int pieceCounter = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
        {
            if (std::isalpha(fen[i][j])) {
                sf::Sprite& pieceSprite = pieceSprites[pieceCounter];
                PieceSharedPtr piece = std::make_shared<Piece>(pieceSprite, fen[i][j]);
                pieceSprite.setScale(pieceSize / piece->getTexture().getSize().x, pieceSize / piece->getTexture().getSize().y);
                movePiece(piece, i, j);
                pieceSprite = piece->getSprite();
                pieceCounter++;
            }
        }
    }
}

void Board::movePiece(PieceSharedPtr & piece, int row, int col)
{
    const sf::Vector2f boardPos = getSprite().getPosition();
    const sf::FloatRect boardBounds = getSprite().getGlobalBounds();
    float cellSize = (boardBounds.width / 8);
    piece->setPosition(row, col);
    board[row][col] = piece;
    float xPos = boardPos.x + (col * cellSize) + cellSize / 6;
    float yPos = boardPos.y + (row * cellSize) + cellSize / 6;
    piece->getSprite().setPosition(xPos, yPos);
}


