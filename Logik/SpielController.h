#ifndef SPIELCONTROLLER_H
#define SPIELCONTROLLER_H

#include <iostream>
#include <memory>
#include <string>
#include "Spielfeld.h"
#include "Spieler.h"

class SpielController {

public:
    SpielController();

    void spiele();


    std::pair<Position, Position> getMove(Position from, Position to);


    std::string trim(const std::string& str);

    //Überprüft, ob ein Zug gültig ist
    bool isMoveValid(const Position& from, const Position& to) const;

    void print(const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8> board);


    Spielfeld spielfeld;
    std::shared_ptr<MenschSpieler> SpielerWeiss;
    std::shared_ptr<MenschSpieler> SpielerSchwarz;
};

#endif // SPIELCONTROLLER_H
