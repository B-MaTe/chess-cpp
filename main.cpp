#include <iostream>
#include <SFML/Graphics.hpp>
#include "board/Board.hpp"
#include "helper/FenHelper.hpp"
#include "game/Game.hpp"

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

int main()
{
    const sf::Color bgColor(111, 111, 111);
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Chess Game", sf::Style::Fullscreen);
    std::string boardPath = "../static/img/board/board_lighter_cropped.png";
    sf::Sprite boardSprite;
    Board board = Board(boardPath, window.getSize(), boardSprite);
    Game game = Game(board.getBoard(), 0, true,  true);
    std::array<std::array<char, 8>, 8> startPosition = FenHelper::loadFen("1kr5/1pr4p/1b3qp1/5p2/2N5/1P1Q1P2/P1P1N1PP/1K1R4");
    std::array<sf::Sprite, 64> pieceSprites;
    board.loadPosition(startPosition, pieceSprites);
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
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2<int> mousePos = sf::Mouse::getPosition(window);
                    selectedPiece = clickedAtPiece(mousePos, board);
                    if (selectedPiece != PieceSharedPtr(nullptr)) {
                        board.colorSquares(game.getValidMovesForPiece(selectedPiece));
                        selectedPiece->setActive(true);
                        initialPosition = selectedPiece->getSprite().getPosition();
                        isDragging = true;
                        offset = initialPosition - static_cast<sf::Vector2f>(mousePos);
                    }

                }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                board.resetAvailablePositionBackgrounds();
                if (event.mouseButton.button == sf::Mouse::Left && isDragging && selectedPiece != nullptr) {
                    sf::Vector2<int> mousePos = sf::Mouse::getPosition(window);
                    selectedPiece->setActive(false);

                    if (!board.getSprite().getGlobalBounds().contains(mousePos.x, mousePos.y)
                        || !game.validMove(selectedPiece, board.normalizePosition(mousePos.x, mousePos.y))
                        || !board.normalizePositionAndMovePiece(selectedPiece->getSprite().getPosition(), initialPosition, mousePos)) {
                            selectedPiece->getSprite().setPosition(initialPosition);
                    } else {
                        game.afterMove();
                    }

                    isDragging = false;
                    selectedPiece = nullptr;
                }
            }
        }

        if (isDragging && selectedPiece) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            selectedPiece->getSprite().setPosition(static_cast<sf::Vector2f>(mousePosition) + offset);
        }

        window.clear(bgColor);
        board.render(window);


        window.display();
    }

    return 0;
}
