//
// Created by mate on 2023.10.16.
//

#ifndef CHESS_GAME_HPP
#define CHESS_GAME_HPP


#include "../board/Board.hpp"
#include "../enums/Status.cpp"
#include <algorithm>

class Game
{
private:
    int move = 0;
    bool whiteTurn = true;
    bool whiteAtBottom = true;
    bool check = false;
    PieceSharedPtr (&board)[8][8];
public:

    bool static sameColourPieces(PieceSharedPtr & piece1, PieceSharedPtr & piece2);
    bool static nullPtrPiece(const PieceSharedPtr & piece);
    static bool onTable(int row, int col);
    static bool onTable(PiecePosition position)  {
        return onTable(position.row, position.col);
    }
    bool checkMateCanHappen() const;
    bool pieceTurn(PieceSharedPtr & piece) const;
    Game(PieceSharedPtr (&board)[8][8], int move, bool whiteTurn, bool whiteAtBottom);
    void increaseMove() {
        move++;
    }
    [[nodiscard]] int getMove() const {
        return move;
    }
    [[nodiscard]] bool isWhiteAtBottom() const {
        return whiteAtBottom;
    }
    void changeTurn() {
        whiteTurn = !whiteTurn;
    }
    [[nodiscard]] bool isWhiteTurn() const {
        return whiteTurn;
    }
    [[nodiscard]] PieceSharedPtr * getPieceOnTable(char name) const;
    [[nodiscard]] bool isCheckPosition(bool whiteKing) const;
    [[nodiscard]] Status getStatusOfPosition(bool whiteKing) const;
    void afterMove();
    bool validMove(PieceSharedPtr & piece, PiecePosition newPosition, bool lookForCheck) const;
    bool isValidBishopMove(PieceSharedPtr & piece, PiecePosition newPosition) const;
    bool isValidRookMove(PieceSharedPtr & piece, PiecePosition newPosition) const;
    bool isValidQueenMove(PieceSharedPtr & piece, PiecePosition newPosition) const;
    bool isValidKnightMove(PieceSharedPtr & piece, PiecePosition newPosition) const;
    bool isValidKingMove(PieceSharedPtr & piece, PiecePosition newPosition) const;
    bool isValidPawnMove(PieceSharedPtr & piece, PiecePosition newPosition) const;
    std::vector<PiecePosition> getValidMovesForPiece(PieceSharedPtr & piece) const;
    [[nodiscard]] PieceSharedPtr & getSquare(PiecePosition position) const
    {
        return board[position.row][position.col];
    }
    [[nodiscard]] bool emptyPosition(PiecePosition position) const {
        return nullPtrPiece(board[position.row][position.col]);
    }
    bool isCheck() const {
        return check;
    }
    void setCheck(bool isCheck) {
        check = isCheck;
    }
};


#endif //CHESS_GAME_HPP
