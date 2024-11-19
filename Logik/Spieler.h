#ifndef SPIELER_H
#define SPIELER_H

#include <iostream>
#include "Spielfiguren.h"
#include "Spielfeld.h"



class Spieler {
public:

    //Konstruktor
    Spieler(PieceColor color) : color(color) {}
    virtual ~Spieler() = default;

    PieceColor getColor() const { return color; }

protected:
    PieceColor color;
};

// Implementierung für einen menschlichen Spieler (zum Beispiel über eine Konsoleingabe)
class MenschSpieler : public Spieler {
public:
    MenschSpieler(PieceColor color) : Spieler(color) {}

};

// KI ?


#endif // SPIELER_H
