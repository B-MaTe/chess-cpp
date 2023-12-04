//
// Created by mate on 2023.10.14.
//

#pragma once
#ifndef CHESS_FENHELPER_HPP
#define CHESS_FENHELPER_HPP

#include <array>
#include <vector>
#include <iostream>
#include <string>
#include <regex>

#ifndef BASE_FEN
#define BASE_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#endif

class FenHelper
{
public:
    static std::array<std::array<char, 8>, 8> loadFen();
    static std::array<std::array<char, 8>, 8> loadFen(const std::string &fen);
};


#endif //CHESS_FENHELPER_HPP
