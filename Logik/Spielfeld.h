#ifndef SPIELFELD_H
#define SPIELFELD_H

#include <array>
#include <memory>
#include <sstream>
#include "Spielfiguren.h"


class Spielfeld {
public:


    //Konstruktor
    Spielfeld() {}

    // Initialisiert das Schachbrett mit den Standardpositionen
    void initialize(std::array<std::array<std::shared_ptr<Spielfiguren>, cols>, rows> &board);

    void doMove(const Position &from, const Position &to, std::array<std::array<std::shared_ptr<Spielfiguren>, cols>, rows> &board);

    bool isCheck(std::array<std::array<std::shared_ptr<Spielfiguren>, cols>, rows> &board, PieceColor kingColor) const;

    //bool canAttackKing(std::shared_ptr<Spielfiguren> piece, Position kingPos, std::array<std::array<std::shared_ptr<Spielfiguren>, cols>, rows> &board) const;

    //Position findPiecePosition(std::shared_ptr<Spielfiguren> piece, std::array<std::array<std::shared_ptr<Spielfiguren>, cols>, rows> &board) const;

    bool isGameOver(std::array<std::array<std::shared_ptr<Spielfiguren>, cols>, rows> &board, PieceColor kingColor) const;

    bool isPathClear(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) const;

    LastMove lastMove;

    //Schachbrett mit Smartpointern auf Spielfiguren
    std::array<std::array<std::shared_ptr<Spielfiguren>, cols>, rows> board;

};



#endif // SPIELFELD_H
