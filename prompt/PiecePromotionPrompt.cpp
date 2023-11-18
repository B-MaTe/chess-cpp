//
// Created by mate on 2023.11.01.
//

#include "PiecePromotionPrompt.hpp"

PiecePromotionPrompt::PiecePromotionPrompt(bool white, PiecePosition promotingPosition, std::array<sf::Sprite *, 8> promotionPieceSprites) : white(white),
                                                                                                                                             promotingPosition(promotingPosition),
                                                                                                                                           promoteOptions(
        PromoteOptions(
        *promotionPieceSprites[white ? 0 : 4],
        *promotionPieceSprites[white ? 1 : 5],
        *promotionPieceSprites[white ? 2 : 6],
        *promotionPieceSprites[white ? 3 : 7]
        )
    )
{
    promoteOptions.knightSprite.setScale(0.25, 0.25);
    promoteOptions.bishopSprite.setScale(0.25, 0.25);
    promoteOptions.rookSprite.setScale(0.25, 0.25);
    promoteOptions.queenSprite.setScale(0.25, 0.25);

    promoteOptions.knightSprite.setPosition(10, 100);
    promoteOptions.bishopSprite.setPosition(80, 100);
    promoteOptions.rookSprite.setPosition(150, 100);
    promoteOptions.queenSprite.setPosition(220, 100);

}

void PiecePromotionPrompt::showPrompt(sf::RenderWindow &window, sf::Event& event)
{
    detectClickedSprite(window, event);
    window.draw(promoteOptions.knightSprite);
    window.draw(promoteOptions.bishopSprite);
    window.draw(promoteOptions.rookSprite);
    window.draw(promoteOptions.queenSprite);
}

void PiecePromotionPrompt::detectClickedSprite(sf::RenderWindow& window, sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        if (promoteOptions.queenSprite.getGlobalBounds().contains(mousePos)) {
            clickedPiece = white ? 'Q' : 'q';
        } else if (promoteOptions.rookSprite.getGlobalBounds().contains(mousePos)) {
            clickedPiece = white ? 'R' : 'r';
        } else if (promoteOptions.bishopSprite.getGlobalBounds().contains(mousePos)) {
            clickedPiece = white ? 'B' : 'b';
        } else if (promoteOptions.knightSprite.getGlobalBounds().contains(mousePos)) {
            clickedPiece = white ? 'N' : 'n';
        }
    }
}


