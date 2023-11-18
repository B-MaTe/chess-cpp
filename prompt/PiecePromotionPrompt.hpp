//
// Created by mate on 2023.11.01.
//

#ifndef CHESS_PIECE_PROMOTION_PROMPT_HPP
#define CHESS_PIECE_PROMOTION_PROMPT_HPP

#pragma once
#include <string>
#include "../piece/Piece.hpp"

class PiecePromotionPrompt
{
    typedef struct {
        sf::Sprite& knightSprite;
        sf::Sprite& bishopSprite;
        sf::Sprite& rookSprite;
        sf::Sprite& queenSprite;
    } PromoteOptions;
private:
    PiecePosition promotingPosition;
    PromoteOptions promoteOptions;
    char clickedPiece = '-';
    bool white;
    void detectClickedSprite(sf::RenderWindow &window, sf::Event& event);
public:
    PiecePromotionPrompt(bool white, PiecePosition promotingPosition, std::array<sf::Sprite *, 8> promotionPieceSprites);
    void showPrompt(sf::RenderWindow &window, sf::Event& event);
    [[nodiscard]] char getClickedPiece() const {
        return clickedPiece;
    }
    [[nodiscard]] PiecePosition getPromotingPawnPosition() const {
        return promotingPosition;
    }
};


#endif //CHESS_PIECE_PROMOTION_PROMPT_HPP
