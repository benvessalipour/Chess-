#ifndef SPIELFIGUREN_H
#define SPIELFIGUREN_H

#include <string>
#include <iostream>
#include <vector>
//#include "Spielfeld.h"


enum class PieceType{King, Queen, Rook, Bishop, Knight, Pawn};

enum class PieceColor {White, Black};

class Position
{
    public:
        int x; // Spaltennummer (0-7)
        int y; // Zeilennummer (0-7)
};

constexpr size_t rows = 8;
constexpr size_t cols = 8;


class Spielfiguren {
public:

    //Konstruktor
    Spielfiguren(PieceType type, PieceColor color) : type(type), color(color) {}

    // Ändere die Signatur von isMoveValid
    virtual bool isMoveValid(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) = 0;

    virtual ~Spielfiguren() = default;

    //Getter Funktionen
    PieceType getType() const { return type;}
    PieceColor getColor() const { return color;}

    PieceType type;
    PieceColor color;
};

//Konkrete Figur: Bauer
class Pawn : public Spielfiguren {
public:
    Pawn(PieceColor color) : Spielfiguren(PieceType::Pawn, color) {}

    // Korrigierter Methodenname (muss mit dem der Basisklasse übereinstimmen)
    bool isMoveValid(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
        // Beispiel für die Bauernzuglogik
        int direction = (color == PieceColor::White) ? 1 : -1; // Richtung abhängig von der Farbe (Weiß oder Schwarz)

        // Überprüfen, ob der Bauer einen Schritt vorwärts geht
        if (from.x == to.x && (to.y == from.y + direction)) {
            // Überprüfen, ob das Ziel frei ist
            return board[to.y][to.x] == nullptr;
        }

        // Zusätzliche Logik für das Schlagen, Doppelschritt, etc.
        return false;
    }
};

class Knight : public Spielfiguren {
public:
    Knight(PieceColor color) : Spielfiguren(PieceType::Knight, color) {}

    // Implementiere die Springerzuglogik
    bool isMoveValid(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
        int dx = std::abs(from.x - to.x);
        int dy = std::abs(from.y - to.y);
        // Springer bewegt sich in einem "L"-Muster: 2 Felder in eine Richtung und 1 Feld in die andere
        return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
    }
};

// Klasse für Läufer (Bishop)
class Bishop : public Spielfiguren {
public:
    Bishop(PieceColor color) : Spielfiguren(PieceType::Bishop, color) {}

    // Implementiere die Zuglogik für den Läufer
    bool isMoveValid(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
        // Läufer bewegt sich diagonal (Abstand in x und y muss gleich sein)
        if (std::abs(from.x - to.x) == std::abs(from.y - to.y)) {
            return isPathClear(from, to, board); // Stelle sicher, dass der Pfad frei ist
        }
        return false;
    }

private:
    // Überprüfung, ob der diagonale Pfad frei ist
    bool isPathClear(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) {
        int dx = (to.x > from.x) ? 1 : -1;
        int dy = (to.y > from.y) ? 1 : -1;
        int x = from.x + dx;
        int y = from.y + dy;

        while (x != to.x && y != to.y) {
            if (board[y][x] != nullptr) {
                return false; // Es steht eine Figur im Weg
            }
            x += dx;
            y += dy;
        }
        return true; // Der Pfad ist frei
    }
};


class Rook : public Spielfiguren{
public:
    Rook(PieceColor color) : Spielfiguren(PieceType::Rook, color) {}

    // Implementierung der Zuglogik für den Turm
    bool isMoveValid(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
        // Ein Turm bewegt sich entweder horizontal oder vertikal
        if (from.x == to.x || from.y == to.y) {
            return isPathClear(from, to, board); // Stelle sicher, dass der Pfad frei ist
        }
        return false;
    }

private:
    // Hilfsfunktion zur Überprüfung, ob der Pfad zum Ziel frei ist
    bool isPathClear(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) {
        // Implementiere die Logik, um zu prüfen, ob keine Figuren zwischen `from` und `to` stehen.
        // Beispiel für horizontale Bewegung (überprüfe vertikale ebenso)
        if (from.x == to.x) {
            int step = (to.y > from.y) ? 1 : -1;
            for (int y = from.y + step; y != to.y; y += step) {
                if (board[y][from.x] != nullptr) {
                    return false; // Es steht eine Figur im Weg
                }
            }
        } else if (from.y == to.y) {
            int step = (to.x > from.x) ? 1 : -1;
            for (int x = from.x + step; x != to.x; x += step) {
                if (board[from.y][x] != nullptr) {
                    return false; // Es steht eine Figur im Weg
                }
            }
        }
        return true; // Der Pfad ist frei
    }
};

// Klasse für Dame (Queen)
class Queen : public Spielfiguren {
public:
    Queen(PieceColor color) : Spielfiguren(PieceType::Queen, color) {}

    // Implementiere die Zuglogik für die Dame
    bool isMoveValid(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
        // Die Dame bewegt sich entweder diagonal (wie der Läufer) oder gerade (wie der Turm)
        if (from.x == to.x || from.y == to.y) {
            return isPathClear(from, to, board); // Gerade Bewegung
        }
        if (std::abs(from.x - to.x) == std::abs(from.y - to.y)) {
            return isPathClear(from, to, board); // Diagonale Bewegung
        }
        return false;
    }

private:
    // Überprüfung, ob der Pfad frei ist (entweder gerade oder diagonal)
    bool isPathClear(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) {
        if (from.x == to.x) { // Vertikale Bewegung
            int step = (to.y > from.y) ? 1 : -1;
            for (int y = from.y + step; y != to.y; y += step) {
                if (board[y][from.x] != nullptr) {
                    return false; // Es steht eine Figur im Weg
                }
            }
        } else if (from.y == to.y) { // Horizontale Bewegung
            int step = (to.x > from.x) ? 1 : -1;
            for (int x = from.x + step; x != to.x; x += step) {
                if (board[from.y][x] != nullptr) {
                    return false; // Es steht eine Figur im Weg
                }
            }
        } else { // Diagonale Bewegung (wie der Läufer)
            int dx = (to.x > from.x) ? 1 : -1;
            int dy = (to.y > from.y) ? 1 : -1;
            int x = from.x + dx;
            int y = from.y + dy;

            while (x != to.x && y != to.y) {
                if (board[y][x] != nullptr) {
                    return false; // Es steht eine Figur im Weg
                }
                x += dx;
                y += dy;
            }
        }
        return true; // Der Pfad ist frei
    }
};


// Klasse für König (King)
class King : public Spielfiguren {
public:
    King(PieceColor color) : Spielfiguren(PieceType::King, color) {}

    // Implementiere die Zuglogik für den König
    bool isMoveValid(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
        // König kann sich ein Feld in jede Richtung bewegen
        int dx = std::abs(from.x - to.x);
        int dy = std::abs(from.y - to.y);

        return (dx <= 1 && dy <= 1);
    }
};

#endif // SPIELFIGUREN_H
