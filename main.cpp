#include <iostream>
#include <SFML/Graphics.hpp>
#include "board/Board.hpp"
#include "piece/Piece.hpp"
#include "helper/FenHelper.hpp"
#include <vector>

int main()
{
    const sf::Color bgColor(111, 111, 111);
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Chess Game", sf::Style::Fullscreen);
    std::string boardPath = "../static/img/board/board_lighter_cropped.png";
    sf::Sprite boardSprite;
    Board board = Board(boardPath, window.getSize(), boardSprite);
    std::array<std::array<char, 8>, 8> startPosition = FenHelper::loadFen("1k1r4/pbp3p1/1p1q1p1p/5p2/N1Q5/5P1P/4BNP1/R5K1");
    std::array<sf::Sprite, 64> pieceSprites;
    board.loadPosition(startPosition, pieceSprites);

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
            }
        }

        window.clear(bgColor);

        // draw table and pieces
        board.render(window);


        window.display();
    }

    return 0;
}
