#include "Spielfeld.h"
#include "Spielfiguren.h"
#include <memory>
#include <QDebug>



void Spielfeld::initialize(std::array<std::array<std::shared_ptr<Spielfiguren>, cols>, rows> &board) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            // Fülle die zweite Reihe (Reihe 1) mit weißen Bauern
            if (i == 1) {
                board[i][j] = std::make_shared<Pawn>(PieceColor::White);
            }
            // Fülle die siebte Reihe (Reihe 6) mit schwarzen Bauern
            else if (i == 6) {
                board[i][j] = std::make_shared<Pawn>(PieceColor::Black);
            }
            else {
                board[i][j] = nullptr;  // Keine Figur auf diesem Feld
            }
        }
    }

    // Füge andere Figuren hinzu (z.B. Türme, Könige, Damen, usw.) in ihren Startpositionen
    board[0][0] = std::make_shared<Rook>(PieceColor::White);
    board[0][1] = std::make_shared<Knight>(PieceColor::White);
    board[0][2] = std::make_shared<Bishop>(PieceColor::White);
    board[0][3] = std::make_shared<Queen>(PieceColor::White);
    board[0][4] = std::make_shared<King>(PieceColor::White);
    board[0][5] = std::make_shared<Bishop>(PieceColor::White);
    board[0][6] = std::make_shared<Knight>(PieceColor::White);
    board[0][7] = std::make_shared<Rook>(PieceColor::White);

    board[7][0] = std::make_shared<Rook>(PieceColor::Black);
    board[7][1] = std::make_shared<Knight>(PieceColor::Black);
    board[7][2] = std::make_shared<Bishop>(PieceColor::Black);
    board[7][3] = std::make_shared<Queen>(PieceColor::Black);
    board[7][4] = std::make_shared<King>(PieceColor::Black);
    board[7][5] = std::make_shared<Bishop>(PieceColor::Black);
    board[7][6] = std::make_shared<Knight>(PieceColor::Black);
    board[7][7] = std::make_shared<Rook>(PieceColor::Black);

}

void Spielfeld::doMove(const Position &from, const Position &to, std::array<std::array<std::shared_ptr<Spielfiguren>, cols>, rows> &board){
    auto piece = board[from.y][from.x];

    // Setze die Figur an die Zielposition
    board[to.y][to.x] = piece;

    // Leere die Startposition
    board[from.y][from.x] = nullptr;
}


bool Spielfeld::isCheck(std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board, PieceColor kingColor) const {

    PieceColor attackerColor = (kingColor == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    Position kingPos;
    bool kingFound = false;

    // Schritt 1: Finde den König des aktuellen Spielers
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto piece = board[y][x];
            if (piece && piece->getType() == PieceType::King && piece->getColor() == kingColor) {
                kingPos = {x, y};
                kingFound = true;
                break;
            }
        }
        if (kingFound) break;
    }

    if (!kingFound) {
        // Kein König gefunden, Fehler
        return false;
    }

    // Schritt 2: Überprüfe, ob der König von einer gegnerischen Figur angegriffen wird
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto piece = board[y][x];
            if (piece && piece->getColor() == attackerColor) {

                // Spezieller Fall für Bauern (greifen diagonal an)
                if (piece->getType() == PieceType::Pawn) {
                    int pawnDirection = (attackerColor == PieceColor::White) ? -1 : 1; // Weiße Bauern greifen nach oben an, schwarze nach unten
                    if ((kingPos.x == x + 1 && kingPos.y == y + pawnDirection) ||
                        (kingPos.x == x - 1 && kingPos.y == y + pawnDirection)) {
                        return true; // König ist im Schach durch einen Bauern
                    }
                }

                // Für alle anderen Figuren überprüfen wir, ob sie den König angreifen können
                if (piece->isMoveValid({x, y}, kingPos, board)) {
                    return true; // König ist im Schach durch diese Figur
                }
            }
        }
    }

    return false; // Kein Schach gefunden
}

bool Spielfeld::isGameOver(std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board, PieceColor kingColor) const {

    PieceColor attackerColor = (kingColor == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    Position kingPos;
    bool kingFound = false;
    qDebug()<< "GameIsOver wird aufgerufen";

    // Schritt 1: Finde den König des aktuellen Spielers
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto piece = board[y][x];
            if (piece && piece->getType() == PieceType::King && piece->getColor() == kingColor) {
                kingPos = {x, y};
                kingFound = true;
                break;
            }
        }
        if (kingFound) break;
    }

    if (!kingFound) {
        // Kein König gefunden, Fehler
        return false;
    }


    // Schritt 2: Überprüfe, ob der König im Schach steht
    if (!isCheck(board, kingColor)) {
        qDebug() << "King is not in check, not game over.";
        return false;  // Kein Schach, daher kein Schachmatt
    }

    // Schritt 3: Prüfe, ob der König sich durch einen Zug retten kann
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;  // König bleibt auf derselben Position

            int newX = kingPos.x + dx;
            int newY = kingPos.y + dy;

            // Überprüfe, ob die neuen Koordinaten innerhalb des Spielfelds sind
            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                // Speichere die ursprüngliche Position des Königs
                auto movedPiece = board[kingPos.y][kingPos.x];

                // Überprüfe, ob das Zielfeld leer ist oder von einer gegnerischen Figur besetzt ist
                if (!board[newY][newX] || board[newY][newX]->getColor() != kingColor) {
                    // Simuliere den Zug des Königs
                    auto capturedPiece = board[newY][newX];  // Speichere das Stück auf dem Zielfeld (falls vorhanden)
                    board[newY][newX] = movedPiece;
                    board[kingPos.y][kingPos.x] = nullptr;

                    // Prüfe, ob der König nach dem Zug immer noch im Schach steht
                    bool stillInCheck = isCheck(board, kingColor);

                    // Rückgängig machen des simulierten Zugs
                    board[kingPos.y][kingPos.x] = movedPiece;
                    board[newY][newX] = capturedPiece;

                    // Wenn der König sich aus dem Schach retten kann, ist es kein Schachmatt
                    if (!stillInCheck) {
                        return false;
                    }
                }
            }
        }
    }


    // Schritt 4: Prüfe, ob eine andere befreundete Figur den König retten kann
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto piece = board[y][x];
            // Finde alle Figuren der gleichen Farbe wie der König
            if (piece && piece->getColor() == kingColor && piece->getType() != PieceType::King) {
                // Gehe durch alle Felder, zu denen die Figur ziehen könnte
                for (int targetY = 0; targetY < 8; ++targetY) {
                    for (int targetX = 0; targetX < 8; ++targetX) {

                        // Überprüfe, ob die Bewegung der Figur gültig ist
                        if (piece->isMoveValid({x, y}, {targetX, targetY}, board)) {
                            // Speichere die aktuelle Position und das Zielfeld
                            qDebug() << "Piece: " << " at position = " << targetX << targetY;

                            auto capturedPiece = board[targetY][targetX];
                            board[targetY][targetX] = piece;
                            board[y][x] = nullptr;

                            // Prüfe, ob der König nach diesem Zug immer noch im Schach steht
                            bool stillInCheck = isCheck(board, kingColor);

                            // Rückgängig machen des simulierten Zugs
                            board[y][x] = piece;
                            board[targetY][targetX] = capturedPiece;

                            // Wenn der König nicht mehr im Schach ist, dann ist es kein Schachmatt
                            if (!stillInCheck) {
                                qDebug() << "Allied piece at " << x << ", " << y << " can save the king!";
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    // Wenn alle möglichen Züge den König im Schach lassen, ist es Schachmatt
    qDebug() << "King is in checkmate.";
    return true;
}

bool Spielfeld::isPathClear(const Position& from, const Position& to, const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8>& board) const {
    int dx = (to.x > from.x) ? 1 : (to.x < from.x) ? -1 : 0; // Richtung auf der X-Achse
    int dy = (to.y > from.y) ? 1 : (to.y < from.y) ? -1 : 0; // Richtung auf der Y-Achse

    int x = from.x + dx;
    int y = from.y + dy;

    // Gehe den Weg vom Start (from) bis zum Ziel (to) durch und prüfe, ob dort Figuren stehen
    while (x != to.x || y != to.y) {
        if (board[y][x] != nullptr) {
            return false; // Der
            //Weg ist blockiert, wenn eine Figur gefunden wird
        }
        x += dx;
        y += dy;
    }
    qDebug()<<"Weg frei";
    return true; // Der Weg ist frei
}
