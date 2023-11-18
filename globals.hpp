//
// Created by mate on 2023.11.04.
//

#ifndef CHESS_GLOBALS_HPP
#define CHESS_GLOBALS_HPP

#include <SFML/Graphics.hpp>
#include <utility>

struct ButtonWithAction {
    sf::RectangleShape* button;
    sf::Text* buttonText;
    std::function<void()>* action;
    ButtonWithAction(sf::RectangleShape* btn, sf::Text* btnTxt, std::function<void()>* act)
    : button(btn), buttonText(btnTxt), action(act) {}

    ButtonWithAction(sf::RectangleShape* btn, sf::Text* btnTxt)
            : button(btn), buttonText(btnTxt), action(nullptr) {}
};


struct PromptWithActionButtons {
    sf::Text* promptText;
    std::vector<ButtonWithAction*> buttons;

    PromptWithActionButtons(sf::Text* text, std::vector<ButtonWithAction*> buttonArray)
            : promptText(text), buttons(std::move(buttonArray)) {}
};

struct PiecePosition {
    int row = 0;
    int col = 0;

    bool operator==(const PiecePosition& other) const {
        return (row == other.row) && (col == other.col);
    }

    bool operator!=(const PiecePosition& other) const {
        return !(*this == other);
    }
};

#endif //CHESS_GLOBALS_HPP
