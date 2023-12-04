//
// Created by mate on 2023.11.04.
//

#ifndef CHESS_GLOBALS_HPP
#define CHESS_GLOBALS_HPP

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
