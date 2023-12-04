//
// Created by mate on 2023.10.16.
//

#include "Game.hpp"

bool Game::onTable(int row, int col)
{
    return 0 <= row && row <= 7 && 0 <= col && col <= 7;
}

bool Game::sameColourPieces(PieceSharedPtr & piece1, PieceSharedPtr & piece2)
{
    return !nullPtrPiece(piece1)
           && !nullPtrPiece(piece2)
           && ((isupper(piece1->getName()) && isupper(piece2->getName()))
               || (islower(piece1->getName()) && islower(piece2->getName())));
}


bool Game::nullPtrPiece(const PieceSharedPtr & piece)
{
    return piece == nullptr || piece == PieceSharedPtr(nullptr);
}

Game::Game(PieceSharedPtr (&_board)[8][8], int _move, bool _whiteTurn, bool _whiteAtBottom)
        : board(_board), move(_move), whiteTurn(_whiteTurn), whiteAtBottom(_whiteAtBottom) {
}

void Game::afterMove()
{
    increaseMove();
    changeTurn();
}

bool Game::pieceTurn(PieceSharedPtr &piece) const
{
    return piece->isWhite() ? isWhiteTurn() : !isWhiteTurn();
}

bool Game::validMove(PieceSharedPtr & piece, PiecePosition newPosition, bool lookForCheck) const
{
    bool isValid = piece->getPosition() != newPosition;

    if (isValid) {
        switch(piece->getName()) {
            case 'k':
            case 'K':
                isValid = isValidKingMove(piece, newPosition);
                break;
            case 'q':
            case 'Q':
                isValid = isValidQueenMove(piece, newPosition);
                break;
            case 'r':
            case 'R':
                isValid = isValidRookMove(piece, newPosition);
                break;
            case 'b':
            case 'B':
                isValid = isValidBishopMove(piece, newPosition);
                break;
            case 'n':
            case 'N':
                isValid = isValidKnightMove(piece, newPosition);
                break;
            case 'p':
            case 'P':
                isValid = isValidPawnMove(piece, newPosition);
                break;
            default:
                isValid = false;
        }

        // check validation with simulating move
        if (isValid && lookForCheck) {
            PieceSharedPtr capturedPiece = getSquare(newPosition);
            getSquare(newPosition).reset();
            getSquare(newPosition) = piece;
            PiecePosition oldPosition = piece->getPosition();
            piece->setPosition(newPosition);
            getSquare(oldPosition).reset();
            isValid = !isCheckPosition(piece->isWhite());

            getSquare(newPosition) = capturedPiece;
            piece->setPosition(oldPosition);
            getSquare(piece->getPosition()) = piece;
        }
    }

    return isValid;
}

bool Game::isValidBishopMove(PieceSharedPtr & piece, PiecePosition newPosition) const
{
    const PiecePosition oldPosition = piece->getPosition();
    int rowDiff = std::abs(newPosition.row - oldPosition.row);
    int colDiff = std::abs(newPosition.col - oldPosition.col);

    bool isValid = rowDiff == colDiff;
    bool foundNewPosition = false;

    // to be the most efficient, I check the direction of the move and only look for those positions.

    if (isValid) {
        int rowMultiplier;
        int colMultiplier;

        // left-up
        if (oldPosition.row > newPosition.row && oldPosition.col > newPosition.col) {
            rowMultiplier = -1;
            colMultiplier = -1;
        // right-up
        } else if (oldPosition.row > newPosition.row && oldPosition.col < newPosition.col) {
            rowMultiplier = -1;
            colMultiplier = 1;
        // left-down
        } else if (oldPosition.row < newPosition.row && oldPosition.col > newPosition.col) {
            rowMultiplier = 1;
            colMultiplier = -1;
        // right-down
        } else {
            rowMultiplier = 1;
            colMultiplier = 1;
        }

        PiecePosition positionToCheck = oldPosition;
        while (onTable({positionToCheck.row + rowMultiplier, positionToCheck.col + colMultiplier})) {
            positionToCheck.row += rowMultiplier;
            positionToCheck.col += colMultiplier;
            if (!emptyPosition(positionToCheck)) {
                if (sameColourPieces(piece, getSquare(positionToCheck))) {
                    break;
                }

                foundNewPosition = newPosition == positionToCheck;
                break;
            }

            foundNewPosition = newPosition == positionToCheck;
            if (foundNewPosition) {
                break;
            }
        }
    }

    return isValid && foundNewPosition;
}

bool Game::isValidRookMove(PieceSharedPtr & piece, PiecePosition newPosition) const
{
    const PiecePosition oldPosition = piece->getPosition();
    int rowMultiplier = 0;
    int colMultiplier = 0;
    bool foundNewPosition = false;

    // up
    if (oldPosition.row > newPosition.row) {
        rowMultiplier = -1;
    // down
    } else if (oldPosition.row < newPosition.row) {
        rowMultiplier = 1;
    // left
    } else if (oldPosition.col > newPosition.col) {
        colMultiplier = -1;
    // right
    } else {
        colMultiplier = 1;
    }

    PiecePosition positionToCheck = oldPosition;
    while (onTable({positionToCheck.row + rowMultiplier, positionToCheck.col + colMultiplier})) {
        positionToCheck.row += rowMultiplier;
        positionToCheck.col += colMultiplier;
        if (!emptyPosition(positionToCheck)) {
            if (sameColourPieces(piece, getSquare(positionToCheck))) {
                break;
            }
            foundNewPosition = newPosition == positionToCheck;
            break;
        }

        foundNewPosition = newPosition == positionToCheck;
        if (foundNewPosition) {
            break;
        }
    }

    return foundNewPosition;
}

bool Game::isValidQueenMove(PieceSharedPtr &piece, PiecePosition newPosition) const
{
    return isValidBishopMove(piece, newPosition) || isValidRookMove(piece, newPosition);
}

bool Game::isValidKnightMove(PieceSharedPtr &piece, PiecePosition newPosition) const
{
    const PiecePosition oldPosition = piece->getPosition();
    int rowDiff = std::abs(newPosition.row - oldPosition.row);
    int colDiff = std::abs(newPosition.col - oldPosition.col);

    return ((rowDiff == 1 && colDiff == 2) || (rowDiff == 2 && colDiff == 1))
        && !sameColourPieces(piece, getSquare(newPosition));
}

bool Game::isValidKingMove(PieceSharedPtr &piece, PiecePosition newPosition) const
{
    const PiecePosition oldPosition = piece->getPosition();
    int rowDiff = std::abs(newPosition.row - oldPosition.row);
    int colDiff = std::abs(newPosition.col - oldPosition.col);


    return rowDiff <= 1 && colDiff <= 1 && !sameColourPieces(piece, getSquare(newPosition));
}

bool Game::isValidPawnMove(PieceSharedPtr &piece, PiecePosition newPosition) const
{
    const PiecePosition oldPosition = piece->getPosition();
    const int rowDiff = newPosition.row - oldPosition.row;
    const int colDiff = std::abs(newPosition.col - oldPosition.col);
    bool isValid = colDiff < 2;
    const int direction = (piece->isWhite() ? -1 : 1) * (isWhiteAtBottom() ? 1 : -1);

    if (isValid) {
        // simple move
        if (colDiff == 0) {
            isValid = emptyPosition(newPosition)
                        && (rowDiff == direction
                        || rowDiff == direction * 2
                            && !piece->isMoved()
                            && emptyPosition({newPosition.row - direction, newPosition.col}));
        // capture
        } else {
            isValid = rowDiff == direction && (!emptyPosition(newPosition) && !sameColourPieces(piece, getSquare(newPosition)));
        }
    }

    return isValid;
}

std::vector<PiecePosition> Game::getValidMovesForPiece(PieceSharedPtr &piece) const
{


    // This method is really inefficient, but because it is only used for the gui
    // I left it like this as 'it does its job'

    std::vector<PiecePosition> validPositions;

    if (nullPtrPiece(piece)) {
        return validPositions;
    }

    bool (Game::*validPositionFunc)(PieceSharedPtr &, PiecePosition) const = nullptr;

    switch (piece->getName()) {
        case 'k':
        case 'K':
            validPositionFunc = &Game::isValidKingMove;
            break;
        case 'q':
        case 'Q':
            validPositionFunc = &Game::isValidQueenMove;
            break;
        case 'r':
        case 'R':
            validPositionFunc = &Game::isValidRookMove;
            break;
        case 'b':
        case 'B':
            validPositionFunc = &Game::isValidBishopMove;
            break;
        case 'n':
        case 'N':
            validPositionFunc = &Game::isValidKnightMove;
            break;
        case 'p':
        case 'P':
            validPositionFunc = &Game::isValidPawnMove;
            break;
        default:
            break;
    }

    if (validPositionFunc) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                PiecePosition newPosition = {i, j};
                if (piece->getPosition() != newPosition && (this->*validPositionFunc)(piece, newPosition)) {
                    auto movingPiece = std::make_shared<PieceSharedPtr>(getSquare(piece->getPosition()));
                    auto capturedPiece = std::make_shared<PieceSharedPtr>(getSquare(newPosition));

                    PiecePosition oldPosition = movingPiece->get()->getPosition();
                    movingPiece->get()->setPosition(newPosition);

                    getSquare(newPosition) = *movingPiece;
                    getSquare(oldPosition) = nullptr;

                    if (!isCheckPosition(movingPiece->get()->isWhite())) {
                        validPositions.push_back(newPosition);
                    }

                    movingPiece->get()->setPosition(oldPosition);
                    getSquare(newPosition) = *capturedPiece;
                    getSquare(oldPosition) = *movingPiece;
                }
            }
        }
    }

    return validPositions;
}

bool Game::isCheckPosition(bool whiteKing) const
{
    PieceSharedPtr* king = getPieceOnTable(whiteKing ? 'K' : 'k');

    if (king == nullptr) {
        exit(EXIT_FAILURE);
    }

    for (auto & row : board)
    {
        for (auto & piece: row)
        {
            if (!nullPtrPiece(piece) && !sameColourPieces(*king, piece)) {
                if (validMove(piece, king->get()->getPosition(), false)) {
                    return true;
                }
            }
        }
    }

    return false;
}

Status Game::getStatusOfPosition(bool whiteKing) const
{
    for (auto & row : board)
    {
        for (auto & piece: row)
        {
            if (!nullPtrPiece(piece)
                && (whiteKing ? piece->isWhite() : !piece->isWhite())
                && !getValidMovesForPiece(piece).empty()) {
                return checkMateCanHappen() ? ACTIVE : DRAW;
            }
        }
    }

    return isCheckPosition(whiteKing) ? CHECKMATE : STALEMATE;
}

bool Game::checkMateCanHappen() const {
    int whitePiecesNotKing = 0;
    int blackPiecesNotKing = 0;
    char piecesThatCanMate[] = {'q', 'Q', 'r', 'R', 'p', 'P'};

    for (auto & row : board)
    {
        for (auto & piece: row)
        {
            if (!nullPtrPiece(piece) && piece->getName() != 'k' && piece->getName() != 'K' ) {
                if (std::find(std::begin(piecesThatCanMate), std::end(piecesThatCanMate), piece->getName()) != std::end(piecesThatCanMate)) {
                    std::cout << piece->getName();
                    return true;
                } else {
                    if (piece->isWhite()) {
                        whitePiecesNotKing++;
                    } else {
                        blackPiecesNotKing++;
                    }
                }
            }
        }
    }

    return whitePiecesNotKing > 1 || blackPiecesNotKing > 1;
}

PieceSharedPtr * Game::getPieceOnTable(char name) const
{
    for (auto & i : board) {
        for (auto & j : i) {
            PieceSharedPtr* piece = &j;
            if (!nullPtrPiece(*piece) && piece->get()->getName() == name) {
                return piece;
            }
        }
    }

    return nullptr;
}
