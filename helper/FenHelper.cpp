//
// Created by mate on 2023.10.14.
//

#include "FenHelper.hpp"

std::vector<std::string> getRows(const std::string& piecePositions) {
    std::vector<std::string> rows;
    size_t pos = 0;

    while (pos != std::string::npos) {
        size_t found = piecePositions.find('/', pos);
        if (found != std::string::npos) {
            rows.push_back(piecePositions.substr(pos, found - pos));
            pos = found + 1;
        } else {
            rows.push_back(piecePositions.substr(pos));
            pos = found;
        }
    }

    return rows;
}

int addPiece(const char currentItem, int row, int col, std::array<std::array<char, 8>, 8> &board) {
    int colIndexToAdd;

    if (std::isdigit(currentItem)) {
        colIndexToAdd = std::stoi(std::string(1, currentItem));
    } else if (std::isalpha(currentItem)) {
        board[row][col] = currentItem;
        colIndexToAdd = 1;
    } else {
        exit(EXIT_FAILURE);
    }

    return colIndexToAdd;
}

std::array<std::array<char, 8>, 8> FenHelper::loadFen()
{
    return FenHelper::loadFen(BASE_FEN);
}

std::array<std::array<char, 8>, 8> FenHelper::loadFen(const std::string &fen)
{
    std::array<std::array<char, 8>, 8> board{};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = ' ';
        }
    }

    size_t firstSpacePos = fen.find(' ');
    const std::string piecePositions = firstSpacePos ==
            std::string::npos
                ? fen
                : fen.substr(0, firstSpacePos);
    const std::vector<std::string> rows = getRows(piecePositions);

    int rowIndex = 0;
    int colIndex = 0;
    for (const std::string &row: rows) {
        for (const char currentItem: row) {
            colIndex += addPiece(currentItem, rowIndex, colIndex, board);
        }
        colIndex = 0;
        rowIndex++;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << board[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    return board;
}

