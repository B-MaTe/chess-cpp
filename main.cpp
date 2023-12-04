#include <iostream>
#include <SFML/Graphics.hpp>
#include "board/Board.hpp"
#include "helper/FenHelper.hpp"
#include "game/Game.hpp"
#include "globals.hpp"

PieceSharedPtr clickedAtPiece(sf::Vector2<int> mousePos, Board & board) {
    PieceSharedPtr clickedPiece = nullptr;

    for (auto & row : board.getBoard()) {
        for (const auto & piece : row)
        {
            if (piece != PieceSharedPtr(nullptr)
                && piece->getSprite().getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                clickedPiece = piece;
            }
        }
    }

    return clickedPiece;
}

void colorKingSquareOnChess(Game& game, Board& board) {
    if (game.isCheckPosition(game.isWhiteTurn())) {
        board.colorSquares({game.getPieceOnTable(game.isWhiteTurn() ? 'K' : 'k')->get()->getPosition()});
        game.setCheck(true);
    } else {
        game.setCheck(false);
    }
}

Status checkGameStatus(Game& game) {
    return game.getStatusOfPosition(game.isWhiteTurn());
}

void gameOver(Status status, bool whiteMovedLast) {
    std::cout << "Game Over!" << std::endl;


    switch (status)
    {
        case CHECKMATE:
            std::cout << "Checkmate " << (whiteMovedLast ? "White" : "Black") << " has won the game!" << std::endl;
            break;
        case STALEMATE:
            std::cout << "Stalemate!" << std::endl;
            break;
        case DRAW:
            std::cout << "Draw!" << std::endl;
            break;
        default:
            break;
    }

    exit(0);
}

bool validStartingSide(const char* input) {
    return (strcasecmp(input, "w") == 0 || strcasecmp(input, "b") == 0);
}

int main(int argc, char* argv[])
{
    const sf::Color bgColor(121, 151, 191);
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Chess Game", sf::Style::Fullscreen);
    sf::Font font = sf::Font();
    std::string boardPath = "../static/img/board/board_lighter_cropped.png";
    sf::Sprite boardSprite;
    Status gameStatus;
    Board board = Board(boardPath, window.getSize(), boardSprite);

    bool whiteStarts = !((argc > 2) && validStartingSide(argv[2])) || strcasecmp(argv[2], "w") == 0;
    Game game = Game(board.getBoard(), 0, whiteStarts ,  true);
    std::array<std::array<char, 8>, 8> startPosition = argc > 1 ? FenHelper::loadFen(argv[1]) : FenHelper::loadFen();
    std::array<sf::Sprite, 64> pieceSprites;
    board.loadPosition(startPosition, pieceSprites, game.isWhiteAtBottom(), window);
    bool isDragging = false;
    sf::Vector2f offset;
    sf::Vector2f initialPosition;
    PieceSharedPtr selectedPiece = nullptr;

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    default:
                        break;
                }
            } else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left && board.isActive())
                {
                    sf::Vector2<int> mousePos = sf::Mouse::getPosition(window);
                    selectedPiece = clickedAtPiece(mousePos, board);
                    if (selectedPiece != PieceSharedPtr(nullptr) && game.pieceTurn(selectedPiece)) {
                        board.colorSquares(game.getValidMovesForPiece(selectedPiece));
                        selectedPiece->setCurrentlyMoving(true);
                        initialPosition = selectedPiece->getSprite().getPosition();
                        isDragging = true;
                        offset = initialPosition - static_cast<sf::Vector2f>(mousePos);
                    }
                }
            } else if (event.type == sf::Event::MouseButtonReleased && board.isActive()) {
                if (event.mouseButton.button == sf::Mouse::Left && isDragging && selectedPiece != nullptr) {
                    board.resetAvailablePositionBackgrounds();
                    sf::Vector2<int> mousePos = sf::Mouse::getPosition(window);
                    selectedPiece->setCurrentlyMoving(false);
                    PiecePosition normalizedNewPosition = board.normalizePosition(mousePos.x, mousePos.y);
                    if (!board.getSprite().getGlobalBounds().contains(mousePos.x, mousePos.y)
                        || !game.validMove(selectedPiece, normalizedNewPosition, true)
                        || !board.normalizePositionAndMovePiece(selectedPiece->getSprite().getPosition(), mousePos, game.isWhiteAtBottom(), window)) {
                        selectedPiece->getSprite().setPosition(initialPosition);
                    } else {
                        game.afterMove();
                        gameStatus = checkGameStatus(game);
                    }

                    isDragging = false;
                    selectedPiece = nullptr;
                }
            }
        }

        if (board.isJustPromoted()) {
            board.setJustPromoted(false);
            gameStatus = checkGameStatus(game);
        }

        colorKingSquareOnChess(game, board);

        if (isDragging && selectedPiece && board.isActive()) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            selectedPiece->getSprite().setPosition(static_cast<sf::Vector2f>(mousePosition) + offset);
        }

        window.clear(bgColor);
        board.render(window, event);
        window.display();


        switch(gameStatus) {
            case CHECKMATE:
            case STALEMATE:
            case DRAW:
                gameOver(gameStatus, game.isWhiteTurn());
                break;
            case ACTIVE:
            default:
                break;
        }
    }

    return 0;
}
