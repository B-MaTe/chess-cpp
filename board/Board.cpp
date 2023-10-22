//
// Created by mate on 2023.09.10.
//

#include "Board.hpp"

#include <memory>

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

    if (!availablePositionBackgrounds.empty()) {
        for (const auto &shapePtr : availablePositionBackgrounds) {
            window.draw(*shapePtr);
        }
    }

    PieceSharedPtr activePiece = nullptr;

    for (const auto & row : board) {
        for (const auto & piece : row) {
            if (piece != PieceSharedPtr(nullptr)) {
                if (piece->isActive()) {
                    activePiece = piece;
                } else {
                    window.draw(piece->getSprite());
                }
            }
        }
    }

    if (activePiece != nullptr) {
        window.draw(activePiece->getSprite());
    }
}

void Board::loadPosition(std::array<std::array<char, 8>, 8> fen, std::array<sf::Sprite, 64>& pieceSprites)
{
    for (int i = 0; i < 64; i++)
    {
        pieceSprites[i] = sf::Sprite();
    }

    float pieceSize = getSprite().getGlobalBounds().width / 8 * 0.68;
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

bool Board::movePiece(PieceSharedPtr & piece, int row, int col)
{
    PieceSharedPtr & newPosPiece = board[row][col];
    const sf::FloatRect boardBounds = getSprite().getGlobalBounds();
    float cellSize = (boardBounds.width / 8);

    // Reset Old and New Position.
    board[piece->getPosition().row][piece->getPosition().col].reset();
    newPosPiece.reset();

    piece->setPosition({row, col});
    if (!piece->isMoved()) {
        piece->setMoved(true);
    }

    board[row][col] = piece;
    float xPos = boardBounds.left + (col * cellSize) + cellSize / 6;
    float yPos = boardBounds.top + (row * cellSize) + cellSize / 6;
    piece->getSprite().setPosition(xPos, yPos);
    for (auto & i : board)
    {
        for (auto & j : i)
        {
            std::string pieceStr = " ";
            if (j != PieceSharedPtr(nullptr)) {
                pieceStr = j->getName();
            }
            std::cout << pieceStr + " ";
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------------------------" << std::endl;

    return true;
}

bool Board::normalizePositionAndMovePiece(sf::Vector2f newPosition, sf::Vector2f initialPosition, sf::Vector2i  mousePosition)
{
    PieceSharedPtr pieceSharedPtr = findPieceSharedPtrByPosition(newPosition);
    PiecePosition normalizedPosition = normalizePosition(mousePosition.x, mousePosition.y);
    return movePiece(pieceSharedPtr, normalizedPosition);
}


PieceSharedPtr Board::findPieceSharedPtrByPosition(sf::Vector2f position)
{
    PieceSharedPtr closestPiece;
    for (auto & i : board) {
        for (auto & piece : i)
        {
            if (piece != PieceSharedPtr(nullptr)) {
                if (piece->getSprite().getPosition() == position) {
                    return piece;
                }
            }
        }
    }

    return nullptr;
}

PiecePosition Board::normalizePosition(int x, int y)
{
    const sf::FloatRect boardBounds = getSprite().getGlobalBounds();
    const float cellSize = (boardBounds.width / 8);
    return {
        static_cast<int>((y - boardBounds.top) / cellSize),
        static_cast<int>((x - boardBounds.left) /  cellSize)
        };
}

void Board::colorSquares(const std::vector<PiecePosition>& positions)
{
    const sf::FloatRect boardBounds = getSprite().getGlobalBounds();
    const float cellSize = boardBounds.width / 8;

    for (auto &position : positions) {
        std::shared_ptr<sf::Shape> square;
        bool isBlackSquare = (position.row + position.col) % 2 == 0;

        if (getSquare(position) == PieceSharedPtr(nullptr)) {
            square = std::make_shared<sf::CircleShape>(cellSize / 5);
            square->setPosition({boardBounds.left + (position.col * cellSize) + cellSize / 3.5f, boardBounds.top + (position.row * cellSize) + cellSize / 3.5f });
        } else {
            auto rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(cellSize, cellSize));
            square = rect;
            square->setPosition({boardBounds.left + position.col * cellSize, boardBounds.top + position.row * cellSize});
        }

        square->setFillColor(isBlackSquare ? sf::Color(100, 100, 100) : sf::Color(75, 75, 75));

        availablePositionBackgrounds.push_back(square);
    }
}


