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
    }
}

void createGameOverPrompt(sf::Window& window, sf::Font& font, bool isWhiteWinner, std::vector<PromptWithActionButtons*>& promptsWithActionButtons) {
    exit(0);
    // Commented out code as it produces nullpointerexception, will look into it later.
//    auto windowSize = window.getSize();
//
//    // Initialize and create the promptText
//    sf::Text promptText;
//    promptText.setFont(font);
//    std::string promptStr = "Game Over!\n";
//    promptStr.append(isWhiteWinner ? "White " : "Black ").append("has won the game!\nDo you want to play again or Quit?");
//    promptText.setString(promptStr);
//    promptText.setCharacterSize(24);
//
//    // Initialize and create the quit button
//    sf::Text quitText;
//    quitText.setFont(font);
//    quitText.setString("Quit");
//    quitText.setCharacterSize(15);
//
//    sf::RectangleShape* button;
//    button->setSize({100, 100});
//    button->setPosition({static_cast<float>(windowSize.x / 2), static_cast<float>(windowSize.y / 2)});
//    button->setFillColor(sf::Color::Red);
//
//    // Create the buttons vector and add the quit button
//    std::vector<ButtonWithAction*> buttons;
//    buttons.emplace_back(new ButtonWithAction(button, &quitText));
//
//    // Create the PromptWithActionButtons and add it to the vector
//    promptsWithActionButtons.emplace_back(new PromptWithActionButtons(&promptText, buttons));
}

Status checkGameStatus(Game& game) {
    return game.getStatusOfPosition(game.isWhiteTurn());
}

void renderPrompts(sf::RenderWindow& window, std::vector<PromptWithActionButtons*>& promptsWithActionButtons) {
    for (const auto& promptWithActionButtons : promptsWithActionButtons) {
        if (promptWithActionButtons != nullptr) {
            window.draw(*promptWithActionButtons->promptText);
            for (const auto& buttonWithAction : promptWithActionButtons->buttons) {
                if (buttonWithAction != nullptr) {
                    window.draw(*buttonWithAction->buttonText);
                    window.draw(*buttonWithAction->button);
                }
            }
        }
    }
}

int main()
{
    const sf::Color bgColor(121, 151, 191);
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Chess Game", sf::Style::Fullscreen);
    sf::Font font = sf::Font();
    std::vector<PromptWithActionButtons*> promptsWithActionButtons;
    std::string boardPath = "../static/img/board/board_lighter_cropped.png";
    sf::Sprite boardSprite;
    Status gameStatus;
    Board board = Board(boardPath, window.getSize(), boardSprite);
    Game game = Game(board.getBoard(), 0, true,  true);
    std::array<std::array<char, 8>, 8> startPosition = FenHelper::loadFen("1k6/p1p4P/1p6/8/8/8/1p3PPP/6K1");
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
                        Board::drawBoard(board.getBoard());
                        gameStatus = checkGameStatus(game);
                    }

                    isDragging = false;
                    selectedPiece = nullptr;
                }
            }
        }

        if (board.isJustPromoted()) {
            board.setJustPromoted(false);
            Board::drawBoard(board.getBoard());
            gameStatus = checkGameStatus(game);
        }

        colorKingSquareOnChess(game, board);

        if (isDragging && selectedPiece && board.isActive()) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            selectedPiece->getSprite().setPosition(static_cast<sf::Vector2f>(mousePosition) + offset);
        }

        window.clear(bgColor);
        board.render(window, event);

        renderPrompts(window, promptsWithActionButtons);

        window.display();


        switch(gameStatus) {
            case CHECKMATE:
                std::cout << "checkmate!" << std::endl << std::flush;
                createGameOverPrompt(window, font, game.isWhiteTurn(), promptsWithActionButtons);
                break;
            case STALEMATE:
                std::cout << "stalemate!" << std::endl << std::flush;
                break;
            case ACTIVE:
            default:
                break;
        }
    }

    // delete pointers
    for (const auto& promptWithActionButtons : promptsWithActionButtons) {
        if (promptWithActionButtons != nullptr) {
            delete promptWithActionButtons->promptText;
            for (const auto& button : promptWithActionButtons->buttons) {
                if (button != nullptr) {
                    delete button->button;
                }
            }
        }

    }

    return 0;
}
