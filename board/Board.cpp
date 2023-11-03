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

    for (auto & row : board) {
        for (auto & piece : row) {
            piece = PieceSharedPtr(nullptr);
        }
    }

    const char* promotedPieceMapping = "NBRQnbrq";
    for (int i = 0; i < 8; i++) {
        promotionPieceTextures[i].loadFromFile(Piece::getPath(promotedPieceMapping[i]));
        promotionPieceSprites[i] = new sf::Sprite();
        promotionPieceSprites[i]->setTexture(promotionPieceTextures[i]);
    }
}


Board::~Board()
{
    for (const auto &item: promotionPieceSprites) {
        delete item;
    }
}


void Board::drawBoard(PieceSharedPtr (&boardToPrint)[8][8])
{
    for (auto & i : boardToPrint)
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
}

void Board::render(sf::RenderWindow &window)
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
                if (piece->isCurrentlyMoving()) {
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

    // check for promotion
    if (piecePromotionPrompt != nullptr) {
        if (piecePromotionPrompt->getClickedPiece() == '-') {
            piecePromotionPrompt->showPrompt(window);
        } else {
            promotePawn(getSquare(piecePromotionPrompt->getPromotingPawnPosition()));
            setActive(true);
            delete piecePromotionPrompt;
            piecePromotionPrompt = nullptr;
            setJustPromoted(true);

        }
    }
}

void Board::loadPosition(std::array<std::array<char, 8>, 8> fen, std::array<sf::Sprite, 64>& pieceSprites, bool whiteAtBottom, sf::RenderWindow &window)
{
    for (int i = 0; i < 64; i++)
    {
        pieceSprites[i] = sf::Sprite();
    }

    int pieceCounter = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
        {
            if (std::isalpha(fen[i][j])) {
                sf::Sprite& pieceSprite = pieceSprites[pieceCounter];
                PieceSharedPtr piece = std::make_shared<Piece>(pieceSprite, fen[i][j]);
                setPieceSpriteProperties(pieceSprite, piece);
                piece->setPosition({i, j});
                movePiece(piece, i, j, !piece->isAtStartingSquare(whiteAtBottom), whiteAtBottom, window);
                pieceCounter++;
            }
        }
    }
}


void Board::setPieceSpriteProperties(sf::Sprite &pieceSprite, PieceSharedPtr & piece)
{
    const float pieceSize = getSprite().getGlobalBounds().width / 8 * 0.68;

    sf::Texture pieceTexture = *piece->getSprite().getTexture();
    sf::IntRect textureRect(0, 0,pieceTexture.getSize().x, pieceTexture.getSize().y);
    piece->getSprite().setTextureRect(textureRect);
    pieceSprite.setScale(
            pieceSize / piece->getTexture().getSize().x,
            pieceSize / piece->getTexture().getSize().y);
}

bool Board::movePiece(PieceSharedPtr & piece, int row, int col, bool countsAsMove, bool whiteAtBottom, sf::RenderWindow &window)
{
    PieceSharedPtr & newPosPiece = board[row][col];
    const sf::FloatRect boardBounds = getSprite().getGlobalBounds();
    float cellSize = (boardBounds.width / 8);
    float xPos = boardBounds.left + (col * cellSize) + cellSize / 6;
    float yPos = boardBounds.top + (row * cellSize) + cellSize / 6;

    // Reset Old and New Position.
    board[piece->getPosition().row][piece->getPosition().col].reset();
    newPosPiece.reset();

    piece->setPosition({row, col});
    if (countsAsMove && !piece->isMoved()) {
        piece->setMoved(true);
    }

    board[row][col] = piece;
    piece->getSprite().setPosition(xPos, yPos);


    // check for pawn promotion
    if (countsAsMove && pawnPromotionMove(piece, whiteAtBottom)) {
        piecePromotionPrompt = new PiecePromotionPrompt(piece->isWhite(), piece->getPosition(), promotionPieceSprites);
        setActive(false);
    }

    return true;
}

bool Board::normalizePositionAndMovePiece(sf::Vector2f newPosition, sf::Vector2i  mousePosition, bool whiteAtBottom, sf::RenderWindow &window)
{
    PieceSharedPtr pieceSharedPtr = findPieceSharedPtrByPosition(newPosition);
    PiecePosition normalizedPosition = normalizePosition(mousePosition.x, mousePosition.y);
    return movePiece(pieceSharedPtr, normalizedPosition, true, whiteAtBottom, window);
}

void Board::promotePawn(PieceSharedPtr & piece) {
    // Only called if clicked at piece
    piece->setName(piecePromotionPrompt->getClickedPiece());
    piece->loadFromFile(Piece::getPath(piece->getName()));
    setPieceSpriteProperties(piece->getSprite(), piece);

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

bool Board::pawnPromotionMove(PieceSharedPtr &piece, bool whiteAtBottom)
{
    // this method is always called with the moving piece.
    const char name = piece->getName();
    int row = piece->getPosition().row;

    return tolower(name) == 'p' && (piece->isWhite()
                                ? whiteAtBottom ? row == 0 : 7
                                : whiteAtBottom ? row == 7 : 0);
}


