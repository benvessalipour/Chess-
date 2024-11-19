#ifndef SPIELFIGUREN_H
#define SPIELFIGUREN_H

#include <string>
#include <iostream>
#include <vector>
#include <memory>

enum class PieceType{King, Queen, Rook, Bishop, Knight, Pawn};

enum class PieceColor {White, Black};

class Position {
public:
    int x; // Spaltennummer (0-7)
    int y; // Zeilennummer (0-7)
};

class Spielfiguren;
struct LastMove {
    Position from;
    Position to;
    std::shared_ptr<Spielfiguren> piece;
};

constexpr size_t rows = 8;
constexpr size_t cols = 8;


class Spielfiguren {
public:

    //Konstruktor
    Spielfiguren(PieceType type, PieceColor color) : type(type), color(color) {}

    // Ändere die Signatur von isMoveValid
    virtual bool isMoveValid(const Position& from, const Position& to, std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) = 0;

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

    //Funktion für normale Bewegungen
    bool isMoveValid(const Position& from, const Position& to, std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
        int direction = (color == PieceColor::White) ? 1 : -1;
        int startRow = (color == PieceColor::White) ? 1 : 6;

        // Normaler Zug: Ein Feld nach vorne
        if (from.x == to.x && to.y == from.y + direction && board[to.y][to.x] == nullptr) {
            return true;
        }

        // Doppelschritt: Zwei Felder nach vorne, wenn der Bauer auf seiner Startposition ist
        if (from.x == to.x && from.y == startRow && to.y == from.y + 2 * direction && board[to.y][to.x] == nullptr && board[from.y + direction][to.x] == nullptr) {
            return true;
        }

        // Schlagen: Diagonal um ein Feld, wenn eine gegnerische Figur dort steht
        if (std::abs(from.x - to.x) == 1 && to.y == from.y + direction && board[to.y][to.x] != nullptr && board[to.y][to.x]->getColor() != color) {
            return true;
        }

        return false;
    }

    // Überladen der Methode mit vier Parametern für en Passant
    bool isMoveValid(const Position& from, const Position& to,
                     std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board,
                     const LastMove& lastMove) {
        int direction = (color == PieceColor::White) ? 1 : -1;
        int startRow = (color == PieceColor::White) ? 1 : 6;

        // En passant: Diagonal schlagen, wenn der gegnerische Bauer einen Doppelschritt gemacht hat
        if (std::abs(from.x - to.x) == 1 && to.y == from.y + direction) {
            // Prüfen, ob der letzte Zug ein Doppelschritt eines gegnerischen Bauern war
            if (lastMove.piece != nullptr && lastMove.piece->getType() == PieceType::Pawn &&
                lastMove.to.y == from.y && std::abs(lastMove.from.y - lastMove.to.y) == 2) {
                // Prüfen, ob der angegriffene Bauer direkt neben dem aktuellen Bauer steht
                if (lastMove.to.x == to.x) {
                    board[lastMove.to.x][lastMove.to.y] = nullptr;
                    std::cout << "en passant"<<std::endl;
                    return true;  // En passant ist möglich
                }
            }
        }

        return false;
    }
};


class Knight : public Spielfiguren {
public:
    Knight(PieceColor color) : Spielfiguren(PieceType::Knight, color) {}

    // Implementiere die Springerzuglogik
    bool isMoveValid(const Position& from, const Position& to, std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
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

    bool isMoveValid(const Position& from, const Position& to, std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
        // Läufer bewegt sich diagonal
        if (std::abs(from.x - to.x) == std::abs(from.y - to.y)) {
            return isPathClear(from, to, board) && (board[to.y][to.x] == nullptr || board[to.y][to.x]->getColor() != color);
        }
        return false;
    }

private:
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



class Rook : public Spielfiguren {
public:
    Rook(PieceColor color) : Spielfiguren(PieceType::Rook, color) {}

    bool isMoveValid(const Position& from, const Position& to, std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
        if (from.x == to.x || from.y == to.y) {
            return isPathClear(from, to, board) && (board[to.y][to.x] == nullptr || board[to.y][to.x]->getColor() != color);
        }
        return false;
    }

private:
    bool isPathClear(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) {
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


class Queen : public Spielfiguren {
public:
    Queen(PieceColor color) : Spielfiguren(PieceType::Queen, color) {}

    bool isMoveValid(const Position& from, const Position& to, std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
        if (from.x == to.x || from.y == to.y || std::abs(from.x - to.x) == std::abs(from.y - to.y)) {
            return isPathClear(from, to, board) && (board[to.y][to.x] == nullptr || board[to.y][to.x]->getColor() != color);
        }
        return false;
    }

private:
    bool isPathClear(const Position& from, const Position& to, std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) {
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
        } else { // Diagonale Bewegung
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
    bool isMoveValid(const Position& from, const Position& to, std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) override {
        // König kann sich ein Feld in jede Richtung bewegen
        int dx = std::abs(from.x - to.x);
        int dy = std::abs(from.y - to.y);

        //Normale KÖnigsbewegung
        if (dx <= 1 && dy <= 1) {
            return true;
        }

        // Rochade: Der König bewegt sich um zwei Felder horizontal
        if (dx == 2 && dy == 0) {
            // Prüfe, ob der König sich bewegt hat
            if (hasMoved) {
                return false;
            }

            // Prüfe, ob es sich um eine kurze oder lange Rochade handelt
            bool isKingSideCastle = (to.x > from.x); // Kurze Rochade
            bool isQueenSideCastle = (to.x < from.x); // Lange Rochade

            // Für die kurze Rochade: Überprüfen, ob der Turm da ist und sich nicht bewegt hat
            if (isKingSideCastle) {
                // Der Turm für die kurze Rochade steht an (7, y) für Weiß oder (7, y) für Schwarz
                auto rook = board[from.y][7];
                if (!rook || rook->getType() != PieceType::Rook || rook->getColor() != color) {
                    return false;
                }

                // Prüfe, ob der Weg frei ist (kein Hindernis zwischen König und Turm)
                for (int x = from.x + 1; x < 7; ++x) {
                    if (board[from.y][x] != nullptr) {
                        return false;
                    }
                }

                // Prüfen, ob der König im Schach steht, durch Schach zieht oder im Schach endet
                if (isInCheck(board, from, {from.x + 1, from.y}) || isInCheck(board, from, to)) {
                    return false;
                }

                return true; // Die kurze Rochade ist gültig
            }

            // Für die lange Rochade: Überprüfen, ob der Turm da ist und sich nicht bewegt hat
            if (isQueenSideCastle) {
                // Der Turm für die lange Rochade steht an (0, y) für Weiß oder (0, y) für Schwarz
                auto rook = board[from.y][0];
                if (!rook || rook->getType() != PieceType::Rook || rook->getColor() != color) {
                    return false;
                }

                // Prüfe, ob der Weg frei ist (kein Hindernis zwischen König und Turm)
                for (int x = from.x - 1; x > 0; --x) {
                    if (board[from.y][x] != nullptr) {
                        return false;
                    }
                }

                // Prüfen, ob der König im Schach steht, durch Schach zieht oder im Schach endet
                if (isInCheck(board, from, {from.x - 1, from.y}) || isInCheck(board, from, to)) {
                    return false;
                }

                return true; // Die lange Rochade ist gültig
            }
        }

        return false;

    }

    // Setze den Status, ob der König sich schon bewegt hat
    void setHasMoved(bool moved) {
        hasMoved = moved;
    }

private:
    bool hasMoved;

    // Prüfe, ob der König durch oder auf ein bedrohtes Feld zieht
    bool isInCheck(std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board, const Position& kingPos, const Position& targetPos) {
        // Prüfen, ob das Ziel oder der Zwischenweg des Königs bedroht ist
        // Diese Funktion muss implementiert werden, um zu überprüfen, ob der König in Gefahr steht
        // Hierfür müsstest du alle gegnerischen Figuren und deren mögliche Züge prüfen.
        // Beispielhaft:
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                auto piece = board[y][x];
                if (piece && piece->getColor() != color && piece->isMoveValid({x, y}, targetPos, board)) {
                    return true; // Ein Gegner kann das Ziel des Königs erreichen
                }
            }
        }
        return false; // Kein Gegner bedroht das Ziel des Königs
    }
};

#endif // SPIELFIGUREN_H
