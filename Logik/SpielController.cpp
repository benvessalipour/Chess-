#include "SpielController.h"
#include "Spielfeld.h"
#include <GUI/mainwindow.h>


SpielController::SpielController()
    : spielfeld(),
      SpielerWeiss(std::make_shared<MenschSpieler>(PieceColor::White)),
      SpielerSchwarz(std::make_shared<MenschSpieler>(PieceColor::Black))
{}

/*void SpielController::spiele() {
    // Initialisiere das Spielfeld
    spielfeld.initialize(spielfeld.board);

    std::cout << "Start:" << std::endl;

    print(spielfeld.board);

    static int i = 0;

    // Hauptspiel-Schleife (solange das Spiel nicht vorbei ist)
    while (!spielfeld.isGameOver(spielfeld.board, PieceColor::Black) && !spielfeld.isGameOver(spielfeld.board, PieceColor::White)) {
        // Simuliere Eingabe für Spieler Weiß
        std::cout << "Spieler Weiß am Zug." << std::endl;

        // Erhalte die Züge von der Funktion getMove() (z.B. in Zukunft von der GUI)
       // std::pair<Position, Position> move = getMove();


        //Position from = move.first;
        //Position to = move.second;

        // Hol die Spielfigur auf der Startposition 'from'
        std::shared_ptr<Spielfiguren> piece = spielfeld.board[from.y][from.x];

        if (piece == nullptr) {
            std::cout << "Keine Spielfigur auf der Position " << from.x << ", " << from.y << ". Versuche es erneut." << std::endl;
            //continue; // Starte den Zug erneut
        }

        // Führe den Zug aus
        spielfeld.doMove(from, to, spielfeld.board);

        print(spielfeld.board);

        // Überprüfe Schach oder Schachmatt
        if (spielfeld.isCheck(spielfeld.board, PieceColor::Black)) {
            std::cout << "Schach!" << std::endl;
        } else {
            std::cout << "Kein Schach"<<std::endl;
        }
        if (spielfeld.isGameOver(spielfeld.board, PieceColor::Black)) {
            std::cout << "Spieler Weiß gewinnt!" << std::endl;
            break;
        }

        //Simuliere Eingabe für Spieler Schwarz
        std::cout << "Spieler Schwarz am Zug." << std::endl;

        i += 1;

        // Wiederhole getMove() für Spieler Schwarz
       // move = getMove();
       // from = move.first;
      //  to = move.second;

        // Hol die Spielfigur auf der Startposition 'from'
        piece = spielfeld.board[from.y][from.x];

        if (piece == nullptr) {
            std::cout << "Keine Spielfigur auf der Position " << from.x << ", " << from.y << ". Versuche es erneut." << std::endl;
            //continue; // Starte den Zug erneut
        }

        // Führe den Zug aus
        spielfeld.doMove(from, to, spielfeld.board);

        // Überprüfe Schach oder Schachmatt
        if (spielfeld.isCheck(spielfeld.board, PieceColor::White)) {
            std::cout << "Schach!" << std::endl;
        }
        if (spielfeld.isGameOver(spielfeld.board, PieceColor::White)) {
            std::cout << "Spieler Schwarz gewinnt!" << std::endl;
            break;
        }

        i += 1;
    }
    return;
}

*/

std::pair<Position, Position> SpielController::getMove(Position from, Position to) {
    return {from, to};
}




//temporäre print funktion
void SpielController::print(const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8> board){

    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            if (spielfeld.board[row][col] != nullptr) {
                // Überprüfe den konkreten Typ der Spielfigur mithilfe von dynamic_cast
                if (std::dynamic_pointer_cast<King>(spielfeld.board[row][col])) {
                    std::cout << "K "; // König
                }
                else if (std::dynamic_pointer_cast<Queen>(spielfeld.board[row][col])) {
                    std::cout << "D "; // Dame
                }
                else if (std::dynamic_pointer_cast<Rook>(spielfeld.board[row][col])) {
                    std::cout << "T "; // Turm
                }
                else if (std::dynamic_pointer_cast<Bishop>(spielfeld.board[row][col])) {
                    std::cout << "L "; // Läufer
                }
                else if (std::dynamic_pointer_cast<Knight>(spielfeld.board[row][col])) {
                    std::cout << "S "; // Springer
                }
                else if (std::dynamic_pointer_cast<Pawn>(spielfeld.board[row][col])) {
                    std::cout << "B "; // Bauer
                }
            } else {
                std::cout << ". "; // Leeres Feld
            }
        }
        std::cout << std::endl;
    }
}


