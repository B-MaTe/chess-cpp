//
// Created by mate on 2023.09.17.
//

#include "Piece.hpp"

std::string Piece::getPath(char _name) {

    const char suffix = islower(_name) ? BLACK_SUFFIX : WHITE_SUFFIX;
    return PIECE_PATH + std::string(1, _name) + suffix + ".png";
}

Piece::Piece(sf::Sprite &initSprite, char _name) : CustomTexture(getPath(_name), initSprite)
{
    position = {0,0};
    name = _name;
    white = isupper(name);
}

bool Piece::isAtStartingSquare(bool whiteAtBottom)
{
    bool atStartingSquare;

    switch (getName()) {
        case 'k':
            atStartingSquare = position == PiecePosition{whiteAtBottom ? 0 : 7, 4};
            break;
        case 'K':
            atStartingSquare = position == PiecePosition{whiteAtBottom ? 7 : 0, 4};
            break;
        case 'q':
            atStartingSquare = position == PiecePosition{whiteAtBottom ? 0 : 7, 3};
            break;
        case 'Q':
            atStartingSquare = position == PiecePosition{whiteAtBottom ? 7 : 0, 3};
            break;
        case 'r':
            atStartingSquare = position == PiecePosition{whiteAtBottom ? 0 : 7, 0}
                    || position == PiecePosition{whiteAtBottom ? 0 : 7, 7};
            break;
        case 'R':
            atStartingSquare = position == PiecePosition{whiteAtBottom ? 7 : 0, 0}
                    || position == PiecePosition{whiteAtBottom ? 7 : 0, 7};
            break;
        case 'b':
            atStartingSquare = position == PiecePosition{whiteAtBottom ? 0 : 7, 2}
                    || position == PiecePosition{whiteAtBottom ? 0 : 7, 5};
            break;
        case 'B':
            atStartingSquare = position == PiecePosition{whiteAtBottom ? 7 : 0, 2}
                    || position == PiecePosition{whiteAtBottom ? 7 : 0, 5};
            break;
        case 'n':
            atStartingSquare = position == PiecePosition{whiteAtBottom ? 0 : 7, 1}
                    || position == PiecePosition{whiteAtBottom ? 0 : 7, 6};
            break;
        case 'N':
            atStartingSquare = position == PiecePosition{whiteAtBottom ? 7 : 0, 1}
                    || position == PiecePosition{whiteAtBottom ? 7 : 0, 6};
            break;
        case 'p':
            atStartingSquare = position.row == (whiteAtBottom ? 1 : 6);
            break;
        case 'P':
            atStartingSquare = position.row == (whiteAtBottom ? 6 : 1);
            break;
        default:
            atStartingSquare = false;
    }
    return atStartingSquare;
}




