#include "GUI/mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QLabel>
#include <Logik/SpielController.h>
#include <Logik/Spielfiguren.h>
#include <Logik/Spielfeld.h>
#include <memory.h>
#include <QMessageBox>
#include <QVBoxLayout>
#include "Netzwerk/GameHandler.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::initializeGame();

    ui->OnOffLabel->text() = "off";
    ui->PPLAbel->setVisible(false);


    ui->TheBackgroundLabel->setPixmap(QPixmap(":/backgrounds/GUI/Pics/ffwf.jpg"));


    _gameHandler = new GameHandler;

    connect(ui->comboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::on_comboBox_activated);
    connect(ui->comboBox_2, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::on_comboBox_2_activated);

    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::on_horizontalSlider_valueChanged);

    lastClickedButton = nullptr;




    QPixmap testPixmap(":/boards/GUI/Pics/OOAD630.jpg");
    if (testPixmap.isNull()) {
        qDebug() << "Failed to load image!";
    } else {
        ui->TheBoardLabel->setPixmap(testPixmap);
        ui->TheBoardLabel->setGeometry(160,130,611,551);
    }





//Sounds
    // Separate audio outputs for each player
    bgMusicAudioOutputA = new QAudioOutput(this);
    bgMusicAudioOutputC = new QAudioOutput(this);
    bgMusicAudioOutputW = new QAudioOutput(this);

    BGMusicA = new QMediaPlayer(this);
    BGMusicC = new QMediaPlayer(this);
    BGMusicW = new QMediaPlayer(this);

    QAudioOutput *moveSoundAudioOutput = new QAudioOutput(this);
    QAudioOutput *captureSoundAudioOutput = new QAudioOutput(this);

    MoveSound = new QMediaPlayer(this);
    CaptureSound  = new QMediaPlayer(this);



    // Set volume for each output
    bgMusicAudioOutputA->setVolume(1.0);  // Full volume for A
    bgMusicAudioOutputC->setVolume(1.0);  // Full volume for C
    bgMusicAudioOutputW->setVolume(1.0);  // Full volume for W

    moveSoundAudioOutput->setVolume(1.0); // Full volume for Move
    captureSoundAudioOutput->setVolume(1.0); // Full volume for Capture




    BGMusicA->setSource(QUrl("qrc:/BGMusic/GUI/Sounds/arabic-music-141627.mp3"));
    BGMusicC->setSource(QUrl("qrc:/BGMusic/GUI/Sounds/chinese-new-year-182996.mp3"));
    BGMusicW->setSource(QUrl("qrc:/BGMusic/GUI/Sounds/dark-country-rock-138300.mp3"));


    MoveSound->setSource(QUrl("qrc:/Sounds/GUI/Sounds/move.mp3"));
    CaptureSound->setSource(QUrl("qrc:/Sounds/GUI/Sounds/capture.mp3"));

    // Assign audio outputs to media players
    BGMusicA->setAudioOutput(bgMusicAudioOutputA);
    BGMusicC->setAudioOutput(bgMusicAudioOutputC);
    BGMusicW->setAudioOutput(bgMusicAudioOutputW);

    MoveSound->setAudioOutput(moveSoundAudioOutput);
    CaptureSound->setAudioOutput(captureSoundAudioOutput);






 //figure size


    QSize FS (60,60);



//black

    blackPawn = QIcon(":/figures/GUI/Pics/black-pawn.png");
    blackRook = QIcon(":/figures/GUI/Pics/black-rook.png");
    blackKnight = QIcon(":/figures/GUI/Pics/black-knight.png");
    blackBishop = QIcon(":/figures/GUI/Pics/black-bishop.png");
    blackQueen = QIcon(":/figures/GUI/Pics/black-queen.png");
    blackKing = QIcon(":/figures/GUI/Pics/black-king.png");






//white

    whitePawn = QIcon(":/figures/GUI/Pics/white-pawn.png");
    whiteRook = QIcon(":/figures/GUI/Pics/white-rook.png");
    whiteKnight = QIcon(":/figures/GUI/Pics/white-knight.png");
    whiteBishop = QIcon(":/figures/GUI/Pics/white-bishop.png");
    whiteQueen = QIcon(":/figures/GUI/Pics/white-queen.png");
    whiteKing = QIcon(":/figures/GUI/Pics/white-king.png");









//black figures

    // Black Pawns
    ui->_2_1->setIcon(blackPawn);
    ui->_2_1->setText("");
    ui->_2_1->setFixedSize(50, 50);
    ui->_2_1->setIconSize(FS);
    ui->_2_1->setStyleSheet("background-color: transparent; border: none;");

    ui->_2_2->setIcon(blackPawn);
    ui->_2_2->setText("");
    ui->_2_2->setFixedSize(50, 50);
    ui->_2_2->setIconSize(FS);
    ui->_2_2->setStyleSheet("background-color: transparent; border: none;");

    ui->_2_3->setIcon(blackPawn);
    ui->_2_3->setText("");
    ui->_2_3->setFixedSize(50, 50);
    ui->_2_3->setIconSize(FS);
    ui->_2_3->setStyleSheet("background-color: transparent; border: none;");

    ui->_2_4->setIcon(blackPawn);
    ui->_2_4->setText("");
    ui->_2_4->setFixedSize(50, 50);
    ui->_2_4->setIconSize(FS);
    ui->_2_4->setStyleSheet("background-color: transparent; border: none;");

    ui->_2_5->setIcon(blackPawn);
    ui->_2_5->setText("");
    ui->_2_5->setFixedSize(50, 50);
    ui->_2_5->setIconSize(FS);
    ui->_2_5->setStyleSheet("background-color: transparent; border: none;");

    ui->_2_6->setIcon(blackPawn);
    ui->_2_6->setText("");
    ui->_2_6->setFixedSize(50, 50);
    ui->_2_6->setIconSize(FS);
    ui->_2_6->setStyleSheet("background-color: transparent; border: none;");

    ui->_2_7->setIcon(blackPawn);
    ui->_2_7->setText("");
    ui->_2_7->setFixedSize(50, 50);
    ui->_2_7->setIconSize(FS);
    ui->_2_7->setStyleSheet("background-color: transparent; border: none;");

    ui->_2_8->setIcon(blackPawn);
    ui->_2_8->setText("");
    ui->_2_8->setFixedSize(50, 50);
    ui->_2_8->setIconSize(FS);
    ui->_2_8->setStyleSheet("background-color: transparent; border: none;");

    // Black Rooks
    ui->_1_1->setIcon(blackRook);
    ui->_1_1->setText("");
    ui->_1_1->setFixedSize(50, 50);
    ui->_1_1->setIconSize(FS);
    ui->_1_1->setStyleSheet("background-color: transparent; border: none;");

    ui->_1_8->setIcon(blackRook);
    ui->_1_8->setText("");
    ui->_1_8->setFixedSize(50, 50);
    ui->_1_8->setIconSize(FS);
    ui->_1_8->setStyleSheet("background-color: transparent; border: none;");

    // Black Knights
    ui->_1_2->setIcon(blackKnight);
    ui->_1_2->setText("");
    ui->_1_2->setFixedSize(50, 50);
    ui->_1_2->setIconSize(FS);
    ui->_1_2->setStyleSheet("background-color: transparent; border: none;");

    ui->_1_7->setIcon(blackKnight);
    ui->_1_7->setText("");
    ui->_1_7->setFixedSize(50, 50);
    ui->_1_7->setIconSize(FS);
    ui->_1_7->setStyleSheet("background-color: transparent; border: none;");

    // Black Bishops
    ui->_1_3->setIcon(blackBishop);
    ui->_1_3->setText("");
    ui->_1_3->setFixedSize(50, 50);
    ui->_1_3->setIconSize(FS);
    ui->_1_3->setStyleSheet("background-color: transparent; border: none;");

    ui->_1_6->setIcon(blackBishop);
    ui->_1_6->setText("");
    ui->_1_6->setFixedSize(50, 50);
    ui->_1_6->setIconSize(FS);
    ui->_1_6->setStyleSheet("background-color: transparent; border: none;");

    // Black Queen
    ui->_1_4->setIcon(blackQueen);
    ui->_1_4->setText("");
    ui->_1_4->setFixedSize(50, 50);
    ui->_1_4->setIconSize(FS);
    ui->_1_4->setStyleSheet("background-color: transparent; border: none;");

    // Black King
    ui->_1_5->setIcon(blackKing);
    ui->_1_5->setText("");
    ui->_1_5->setFixedSize(50, 50);
    ui->_1_5->setIconSize(FS);
    ui->_1_5->setStyleSheet("background-color: transparent; border: none;");







//white Figures


    // White Pawns
    ui->_7_1->setIcon(whitePawn);
    ui->_7_1->setText("");
    ui->_7_1->setFixedSize(50,50);
    ui->_7_1->setIconSize(FS);
    ui->_7_1->setStyleSheet("background-color: transparent; border: none;");

    ui->_7_2->setIcon(whitePawn);
    ui->_7_2->setText("");
    ui->_7_2->setFixedSize(50,50);
    ui->_7_2->setIconSize(FS);
    ui->_7_2->setStyleSheet("background-color: transparent; border: none;");

    ui->_7_3->setIcon(whitePawn);
    ui->_7_3->setText("");
    ui->_7_3->setFixedSize(50,50);
    ui->_7_3->setIconSize(FS);
    ui->_7_3->setStyleSheet("background-color: transparent; border: none;");

    ui->_7_4->setIcon(whitePawn);
    ui->_7_4->setText("");
    ui->_7_4->setFixedSize(50,50);
    ui->_7_4->setIconSize(FS);
    ui->_7_4->setStyleSheet("background-color: transparent; border: none;");

    ui->_7_5->setIcon(whitePawn);
    ui->_7_5->setText("");
    ui->_7_5->setFixedSize(50,50);
    ui->_7_5->setIconSize(FS);
    ui->_7_5->setStyleSheet("background-color: transparent; border: none;");

    ui->_7_6->setIcon(whitePawn);
    ui->_7_6->setText("");
    ui->_7_6->setFixedSize(50,50);
    ui->_7_6->setIconSize(FS);
    ui->_7_6->setStyleSheet("background-color: transparent; border: none;");

    ui->_7_7->setIcon(whitePawn);
    ui->_7_7->setText("");
    ui->_7_7->setFixedSize(50,50);
    ui->_7_7->setIconSize(FS);
    ui->_7_7->setStyleSheet("background-color: transparent; border: none;");

    ui->_7_8->setIcon(whitePawn);
    ui->_7_8->setText("");
    ui->_7_8->setFixedSize(50,50);
    ui->_7_8->setIconSize(FS);
    ui->_7_8->setStyleSheet("background-color: transparent; border: none;");

    // White Rooks
    ui->_8_1->setIcon(whiteRook);
    ui->_8_1->setText("");
    ui->_8_1->setFixedSize(50,50);
    ui->_8_1->setIconSize(FS);
    ui->_8_1->setStyleSheet("background-color: transparent; border: none;");

    ui->_8_8->setIcon(whiteRook);
    ui->_8_8->setText("");
    ui->_8_8->setFixedSize(50,50);
    ui->_8_8->setIconSize(FS);
    ui->_8_8->setStyleSheet("background-color: transparent; border: none;");

    // White Knights
    ui->_8_2->setIcon(whiteKnight);
    ui->_8_2->setText("");
    ui->_8_2->setFixedSize(50,50);
    ui->_8_2->setIconSize(FS);
    ui->_8_2->setStyleSheet("background-color: transparent; border: none;");

    ui->_8_7->setIcon(whiteKnight);
    ui->_8_7->setText("");
    ui->_8_7->setFixedSize(50,50);
    ui->_8_7->setIconSize(FS);
    ui->_8_7->setStyleSheet("background-color: transparent; border: none;");

    // White Bishops
    ui->_8_3->setIcon(whiteBishop);
    ui->_8_3->setText("");
    ui->_8_3->setFixedSize(50,50);
    ui->_8_3->setIconSize(FS);
    ui->_8_3->setStyleSheet("background-color: transparent; border: none;");

    ui->_8_6->setIcon(whiteBishop);
    ui->_8_6->setText("");
    ui->_8_6->setFixedSize(50,50);
    ui->_8_6->setIconSize(FS);
    ui->_8_6->setStyleSheet("background-color: transparent; border: none;");

    // White King
    ui->_8_5->setIcon(whiteKing);
    ui->_8_5->setText("");
    ui->_8_5->setFixedSize(50,50);
    ui->_8_5->setIconSize(FS);
    ui->_8_5->setStyleSheet("background-color: transparent; border: none;");

    // White Queen
    ui->_8_4->setIcon(whiteQueen);
    ui->_8_4->setText("");
    ui->_8_4->setFixedSize(50,50);
    ui->_8_4->setIconSize(FS);
    ui->_8_4->setStyleSheet("background-color: transparent; border: none;");

// all other empty fields

    // Loop through all rows (1 to 8) and columns (1 to 8)
    for (int row = 1; row <= 8; ++row) {
        for (int col = 1; col <= 8; ++col) {
            QString buttonName = QString("_%1_%2").arg(row).arg(col);
            QPushButton *button = findChild<QPushButton *>(buttonName);

            // Skip buttons that are already set for pieces
            if (button->icon().isNull()) {
                button->setIcon(QIcon());
                button->setText("");
                button->setFixedSize(50,50);
                button->setIconSize(FS);
                button->setStyleSheet("background-color: transparent; border: none;");

            }
        }
    }
}

void MainWindow::initializeGame()
{
    controller.spielfeld.initialize(controller.spielfeld.board);
    print(controller.spielfeld.board);  // Print the initial state
}


MainWindow::~MainWindow()
{
    delete ui;

    delete _gameHandler;
    _gameHandler = nullptr;
}




















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                            //
//                              MAIN FUNCTION :)
//                                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void MainWindow::handleButtonClick(QPushButton *button)
{
    if (button == nullptr) {
        qDebug() << "Button is null!";
        return;
    }

    qDebug() << "Button clicked:" << button->objectName();

    if (lastClickedButton == nullptr) {
        if (!button->icon().isNull()) {
            lastClickedButton = button;
            qDebug() << "Stored button as last clicked:" << button->objectName();

            clearHighlights();
            highlightValidMoves(button);

        } else {
            qDebug() << "Button has no icon, cannot select.";
        }
    } else {
        if (lastClickedButton == button) {
            qDebug() << "Deselecting button:" << button->objectName();
            lastClickedButton = nullptr;
            clearHighlights();
        } else {
            if (!lastClickedButton->icon().isNull()) {

                Position from = mapButtonToPosition(lastClickedButton);
                Position to = mapButtonToPosition(button);

                std::shared_ptr<Spielfiguren> piece = controller.spielfeld.board[from.y][from.x];

                if (piece == nullptr) {
                    std::cout << "Keine Spielfigur auf der Position " << from.y << ", " << from.x << ". Versuche es erneut." << std::endl;
                    lastClickedButton = nullptr;
                    return;
                }

                if(piece->getType() == PieceType::Pawn) {

                    //Prüfe en passant
                    auto pawn = std::dynamic_pointer_cast<Pawn>(piece);
                    if(pawn) {
                        if(pawn->isMoveValid(from,to,controller.spielfeld.board)) {

                            controller.spielfeld.doMove(from, to, controller.spielfeld.board);
                            //controller.spielfeld.board[controller.spielfeld.lastMove.to.x][controller.spielfeld.lastMove.to.y] = nullptr;
                            print(controller.spielfeld.board);



                            ///////////////////////////////////////// Move and Capture sound //////////////////////////////////////////////////////////////////////

                            std::shared_ptr<Spielfiguren> targetPiece = controller.spielfeld.board[to.y][to.x];
                            if (!button->icon().isNull() && targetPiece && targetPiece->getColor() != piece->getColor()) {
                                // Play capture sound if the destination contains an opposing piece
                                CaptureSound->play();
                                qDebug() << "Capture sound playing " << CaptureSound->isPlaying();
                            } else {
                                // Play move sound for regular moves
                                MoveSound->play();
                                qDebug() << "Move sound playing " << MoveSound->isPlaying();
                            }


                        } else if (pawn->isMoveValid(from, to, controller.spielfeld.board, controller.spielfeld.lastMove)){


                            controller.spielfeld.doMove(from, to, controller.spielfeld.board);
                            print(controller.spielfeld.board);

                            ///////////////////////////////////////// Move and Capture sound //////////////////////////////////////////////////////////////////////

                            std::shared_ptr<Spielfiguren> targetPiece = controller.spielfeld.board[to.y][to.x];
                            if (!button->icon().isNull() && targetPiece && targetPiece->getColor() != piece->getColor()) {
                                // Play capture sound if the destination contains an opposing piece
                                CaptureSound->play();
                                qDebug() << "Capture sound playing " << CaptureSound->isPlaying();
                            } else {
                                // Play move sound for regular moves
                                MoveSound->play();
                                qDebug() << "Move sound playing " << MoveSound->isPlaying();
                            }

                            //////////////////////////////////////// Graphisches En Passant /////////////////////////////////////////

                            Position enPassantPos = {controller.spielfeld.lastMove.to.x, controller.spielfeld.lastMove.to.y};

                            QPushButton* enPassantButton = findButtonAtPosition(enPassantPos);

                            enPassantButton->setIcon(QIcon());  // Entferne das Icon des geschlagenen Bauern

                        }else if(!pawn->isMoveValid(from, to, controller.spielfeld.board, controller.spielfeld.lastMove)){
                            lastClickedButton = nullptr;
                            return;
                        }
                    }
                    // Speichere den letzten Zug, falls es sich um einen Bauernzug handelt
                    if (piece->getType() == PieceType::Pawn) {
                        controller.spielfeld.lastMove.from = from;
                        controller.spielfeld.lastMove.to = to;
                        controller.spielfeld.lastMove.piece = piece;
                    }

                } else if (piece->isMoveValid(from, to, controller.spielfeld.board)) {


                    //Lokale Move
                    controller.spielfeld.doMove(from, to, controller.spielfeld.board);
                    print(controller.spielfeld.board);

                    ///////////////////////////////////////// Move and Capture sound //////////////////////////////////////////////////////////////////////

                    std::shared_ptr<Spielfiguren> targetPiece = controller.spielfeld.board[to.y][to.x];
                    if (!button->icon().isNull() && targetPiece && targetPiece->getColor() != piece->getColor()) {
                        // Play capture sound if the destination contains an opposing piece
                        CaptureSound->play();
                        qDebug() << "Capture sound playing " << CaptureSound->isPlaying();
                    } else {
                        // Play move sound for regular moves
                        MoveSound->play();
                        qDebug() << "Move sound playing " << MoveSound->isPlaying();
                    }

                    //Schicke Zug über Netzwerk
                    //bool move(Position from, Position to, CONSEQUENCES consequence, PAWN_PROMOTION pawnPromotion);

                    CONSEQUENCES consequence = noCapture;

                    PAWN_PROMOTION pawnpromotion = noPawnPromotion;

                    _gameHandler->move(from, to, consequence, pawnpromotion);

                    // Debugging: Überprüfe Schach-Zustand für beide Farben
                    bool whiteInCheck = controller.spielfeld.isCheck(controller.spielfeld.board, PieceColor::White);
                    bool blackInCheck = controller.spielfeld.isCheck(controller.spielfeld.board, PieceColor::Black);
                    bool whiteInCheckmate = controller.spielfeld.isGameOver(controller.spielfeld.board, PieceColor::White);
                    bool blackInCheckMate = controller.spielfeld.isGameOver(controller.spielfeld.board, PieceColor::Black);

                    if (whiteInCheck) {
                        qDebug() << "Black in check";
                        if (whiteInCheckmate) {
                            // Button/Ton Schachmatt, Spiel vorbei
                            qDebug() << "Checkmate for White!";

                            showGameOverDialog('W');
                        }
                    }

                    if (blackInCheck) {
                        qDebug() << "White in check";
                        if (blackInCheckMate) {
                            // Button/Ton Schachmatt, Spiel vorbei
                            qDebug() << "Checkmate for Black!";


                            showGameOverDialog('B');

                        }
                    }

                } else {

                    //qDebug() << "Figur: " << QString::number(static_cast<int>(piece->getType()));
                    std::cout << "Ungültiger Zug für die Spielfigur " << from.y << ", " << from.x << ". Versuche es erneut." << std::endl;
                    lastClickedButton = nullptr;
                    return;
                }
                if(piece->getType()==PieceType::King && piece->getColor() == PieceColor::Black){
                    //kurze weiße rochade
                    if(from.x == 4 && to.x == 6 && from.y == 7 && from.y == 7){
                        ui->_8_8->setIcon(QIcon());
                        ui->_8_6->setIcon(whiteRook);

                    }
                    //lange weiße rochade
                    if(from.x == 4 && to.x == 2 && from.y == 7 && from.y == 7){
                        ui->_8_1->setIcon(QIcon());
                        ui->_8_4->setIcon(blackRook);
                    }

                }
                if(piece->getType()==PieceType::King && piece->getColor() == PieceColor::White){
                    //kurze schwarze rochade
                    if(from.x == 4 && to.x == 6 && from.y == 0 && from.y == 0){
                        ui->_1_8->setIcon(QIcon());
                        ui->_1_6->setIcon(whiteRook);
                    }
                    //lange schwarze rochade
                    if(from.x == 4 && to.x == 2 && from.y == 0 && from.y == 0){
                        ui->_1_1->setIcon(QIcon());
                        ui->_1_4->setIcon(blackRook);
                    }

                }
                qDebug() << "Moving icon from" << lastClickedButton->objectName() << "to" << button->objectName();
                button->setIcon(lastClickedButton->icon());
                lastClickedButton->setIcon(QIcon());

                clearHighlights();
                lastClickedButton = nullptr;
            } else {
                qDebug() << "Last clicked button has no icon, cannot move icon.";
                lastClickedButton = nullptr;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                            //
//                              MAIN FUNCTION END :(
//                                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                            //
//                              TRANSALTOR FUNCTIONS :)
//                                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//      GUI TO LOGIK

Position MainWindow::mapButtonToPosition(QPushButton *button)
{
    QString buttonName = button->objectName();

    // Example: button names are in the format "_col_row"
    QStringList parts = buttonName.split("_");
    if (parts.size() == 3) {
        int row = parts.at(1).toInt();
        int col = parts.at(2).toInt();
        return Position{col - 1, row - 1};  // Assuming Position uses 0-based indexing
    } else {
        qWarning() << "Invalid button name format:" << buttonName;
        return Position{-1, -1};  // Return an invalid position
    }
}


//      LOGIK TO GUI

QPushButton* MainWindow::findButtonAtPosition(const Position& position) {
    // Assuming your buttons are named as "button_1_1" for (1,1), "button_1_2" for (1,2), etc.
    QString buttonName = QString("_%1_%2").arg(position.y + 1).arg(position.x + 1);  // Adjust for 1-based naming
    QPushButton* button = this->findChild<QPushButton*>(buttonName);

    if (button) {
        return button;
    } else {
        qDebug() << "Button not found at position: " << position.x << ", " << position.y;
        return nullptr;
    }
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                              TRANSALTOR FUNCTIONS END :(
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                              HIGHLIGHTER FUNCTIONS :)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






void MainWindow::highlightValidMoves(QPushButton* button) {
    selectedButton = button;  // Set the selected button

    Position from = mapButtonToPosition(button);
    std::shared_ptr<Spielfiguren> piece = controller.spielfeld.board[from.y][from.x];

    if (piece) {
        // Iterate over all buttons and check for valid moves
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                Position to = {j, i};  // Position on 8x8 board
                QPushButton *targetButton = findButtonAtPosition(to);  // Find button at (i, j)

                if (piece->isMoveValid(from, to, controller.spielfeld.board) ||
                    (piece->getType() == PieceType::Pawn &&
                    std::dynamic_pointer_cast<Pawn>(piece)->isMoveValid(from, to, controller.spielfeld.board, controller.spielfeld.lastMove))) {

                    std::shared_ptr<Spielfiguren> targetPiece = controller.spielfeld.board[to.y][to.x];

                    // If there's a piece on the target and it's an opposing piece, it's a capture
                    if (targetPiece && targetPiece->getColor() != piece->getColor()) {
                        targetButton->setStyleSheet("background-color: rgba(255, 0, 0, 150);");  // Highlight as red (capture)
                    } else {
                        targetButton->setStyleSheet("background-color: rgba(255, 255, 0, 150);");  // Highlight as yellow (valid move)
                    }
                    highlightedButtons.append(targetButton);  // Store for later de-highlighting
                }
            }
        }
        // Highlight the selected button differently
        button->setStyleSheet("background-color: rgba(66, 233, 245, 150);");  // Highlight selected button in cyan
    }
}



void MainWindow::clearHighlights() {
    for (auto& button : highlightedButtons) {
        if (button != selectedButton) {
            button->setStyleSheet("");
            button->setStyleSheet("background-color: transparent; border: none;");
        }
    }
    highlightedButtons.clear();  // Clear the list

    // Reset the style of the selected button if necessary
    if (selectedButton) {
        selectedButton->setStyleSheet("");
        selectedButton->setStyleSheet("background-color: transparent; border: none;");        // Reset style or set a different style if needed
        selectedButton = nullptr;  // Clear the selected button
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                              HIGHLIGHTER FUNCTIONS END :(
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




















////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                              GUI COMMAND FUNCTIONS  :)
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//          CHANGE BOARD AND BACKGROUND

void MainWindow::on_comboBox_activated(int index)
{
    switch(index)
    {
        qDebug() << "ComboBox activated with index:" << index;

    case 0:
        Board = QPixmap(":/boards/GUI/Pics/OOAD630.jpg");
        ui->TheBoardLabel->setPixmap(Board);
        ui->TheBoardLabel->setGeometry(160,130,611,551);

        ui->TheBackgroundLabel->setPixmap(QPixmap(":/backgrounds/GUI/Pics/ffwf.jpg"));
        break;

    case 1:
        Board = QPixmap(":/boards/GUI/Pics/Black and white board.png");
        ui->TheBoardLabel->setPixmap(Board);
        ui->TheBoardLabel->setGeometry(220, 190, 491, 431);

        ui->TheBackgroundLabel->setPixmap(QPixmap(":/backgrounds/GUI/Pics/dqdqdqd.jpg"));
        break;

    case 2:
        Board = QPixmap(":/boards/GUI/Pics/Punk Board.png");
        ui->TheBoardLabel->setPixmap(Board);
        ui->TheBoardLabel->setGeometry(220, 190, 491, 431);

        ui->TheBackgroundLabel->setPixmap(QPixmap(":/backgrounds/GUI/Pics/1000_F_561376397_Airq5FCNa4Ec26GoRXWFxp7VykpMHWPf.jpg"));
        break;


    case 3:
        Board = QPixmap(":/boards/GUI/Pics/Nature board.png");
        ui->TheBoardLabel->setPixmap(Board);
        ui->TheBoardLabel->setGeometry(-26, -26, 982, 862);

        ui->TheBackgroundLabel->setPixmap(QPixmap(":/backgrounds/GUI/Pics/360_F_375500531_WjntKoDjk2R2H7g2uyzoadVs2FjxsDjE.jpg"));

        break;

    default:
        break;

    }

    if (!Board.isNull()) {
        ui->TheBoardLabel->setPixmap(Board);
    } else {
        qDebug() << "Failed to load board image for index:" << index;
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//          CHANGE ONLY BACKGROUND

void MainWindow::on_comboBox_2_activated(int index)
{
    switch(index)
    {
        qDebug() << "ComboBox activated with index:" << index;

    case 0:
        ui->TheBackgroundLabel->setPixmap(QPixmap(":/backgrounds/GUI/Pics/ffwf.jpg"));
        break;

    case 1:
        ui->TheBackgroundLabel->setPixmap(QPixmap(":/backgrounds/GUI/Pics/dqdqdqd.jpg"));
        break;

    case 2:
        ui->TheBackgroundLabel->setPixmap(QPixmap(":/backgrounds/GUI/Pics/1000_F_561376397_Airq5FCNa4Ec26GoRXWFxp7VykpMHWPf.jpg"));
        break;


    case 3:
        ui->TheBackgroundLabel->setPixmap(QPixmap(":/backgrounds/GUI/Pics/360_F_375500531_WjntKoDjk2R2H7g2uyzoadVs2FjxsDjE.jpg"));

        break;

    default:
        break;

    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//      BOARD ON OFF

void MainWindow::on_pushButton_clicked()
{

    if (ui->OnOffLabel->text() == "Off")
    {
        ui->OnOffLabel->setText("On");
        ui->TheBoardLabel->setVisible(false);
        ui->PPLAbel->setVisible(true);

    }
    else
    {
        ui->OnOffLabel->setText("Off");
        ui->TheBoardLabel->setVisible(true);
        ui->PPLAbel->setVisible(false);

    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//      VOLUME SLIDER

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    float volume = static_cast<float>(value)/100;

    bgMusicAudioOutputA->setVolume(volume);
    bgMusicAudioOutputC->setVolume(volume);
    bgMusicAudioOutputW->setVolume(volume);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//      SWITCH MUSIC

void MainWindow::on_comboBox_3_activated(int index)
{
    switch(index)
    {
        qDebug() << "ComboBox activated with index:" << index;

    case 0:
        BGMusicA->play();
        qDebug()<< "BGMusic" << BGMusicA->isPlaying() << BGMusicA->mediaStatus();
        BGMusicC->pause();
        BGMusicW->pause();
        break;

    case 1:
        BGMusicC->play();
        qDebug()<< "BGMusic" << BGMusicC->isPlaying() << BGMusicC->mediaStatus();
        BGMusicA->pause();
        BGMusicW->pause();
        break;

    case 2:
        BGMusicW->play();
        qDebug()<< "BGMusic" << BGMusicW->isPlaying() << BGMusicW->mediaStatus();
        BGMusicC->pause();
        BGMusicA->pause();
        break;


    default:
        break;

    }
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                            //
//                              GUI COMMAND FUNCTIONS END :(
//                                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

























////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                            //
//                              DISPLAY FUNCTIONS  :)
//                                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//          PRINT MATRIX FUNCTION

void MainWindow::print(const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8> board){

    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            if (board[row][col] != nullptr) {
                // Überprüfe den konkreten Typ der Spielfigur mithilfe von dynamic_cast
                if (std::dynamic_pointer_cast<King>(board[row][col])) {
                    std::cout << "K "; // König
                }
                else if (std::dynamic_pointer_cast<Queen>(board[row][col])) {
                    std::cout << "D "; // Dame
                }
                else if (std::dynamic_pointer_cast<Rook>(board[row][col])) {
                    std::cout << "T "; // Turm
                }
                else if (std::dynamic_pointer_cast<Bishop>(board[row][col])) {
                    std::cout << "L "; // Läufer
                }
                else if (std::dynamic_pointer_cast<Knight>(board[row][col])) {
                    std::cout << "S "; // Springer
                }
                else if (std::dynamic_pointer_cast<Pawn>(board[row][col])) {
                    std::cout << "B "; // Bauer
                }
            } else {
                std::cout << ". "; // Leeres Feld
            }
        }
        std::cout << std::endl;
    }
}




//       GAME OVER WINDOW FUNCTION

void showGameOverDialog(char C) {
    // Create a dialog
    QDialog gameOverDialog;
    gameOverDialog.setWindowTitle("Game Over");

    // Set the minimum size for the dialog
    gameOverDialog.setMinimumSize(600, 400);  // Adjust this size as needed

    // Create a layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout(&gameOverDialog);

    QLabel *imageLabel = new QLabel(&gameOverDialog);
    QLabel *textLabel = new QLabel(&gameOverDialog);
    QPushButton *okButton = new QPushButton("OK", &gameOverDialog);

    if(C == 'B') {
        QPixmap pixmap(":/memes/GUI/Pics/black wins.jpg");
        QSize labelSize(550, 350);  // Adjust the size as needed
        QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
        textLabel->setText("Check Mate! Black flexed on you.");
    }
    else if(C == 'W') {
        QPixmap pixmap(":/memes/GUI/Pics/White wins.jpg");
        QSize labelSize(550, 350);  // Adjust the size as needed
        QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
        textLabel->setText("Check Mate! White flexed on you.");
    }

    textLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setAlignment(Qt::AlignCenter);
    okButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Add the widgets to the layout
    layout->addWidget(textLabel);
    layout->addWidget(imageLabel);
    layout->addWidget(okButton);

    // Connect the OK button to the dialog's accept slot
    QObject::connect(okButton, &QPushButton::clicked, &gameOverDialog, &QDialog::accept);

    // Execute the dialog
    gameOverDialog.exec();
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                            //
//                              DISPLAY FUNCTIONS END :(
//                                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






























////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                            //
//                              BOARD BUTTON FUNCTIONS  :)
//                                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





void MainWindow::on__1_1_clicked()
{
    handleButtonClick(ui->_1_1);

}


void MainWindow::on__1_2_clicked()
{
    handleButtonClick(ui->_1_2);
}


void MainWindow::on__1_3_clicked()
{
    handleButtonClick(ui->_1_3);
}


void MainWindow::on__1_4_clicked()
{
    handleButtonClick(ui->_1_4);
}


void MainWindow::on__1_5_clicked()
{
    handleButtonClick(ui->_1_5);
}


void MainWindow::on__1_6_clicked()
{
    handleButtonClick(ui->_1_6);
}


void MainWindow::on__1_7_clicked()
{
    handleButtonClick(ui->_1_7);
}


void MainWindow::on__1_8_clicked()
{
    handleButtonClick(ui->_1_8);
}


void MainWindow::on__2_1_clicked()
{
    handleButtonClick(ui->_2_1);
}

void MainWindow::on__2_2_clicked()
{
    handleButtonClick(ui->_2_2);
}

void MainWindow::on__2_3_clicked()
{
    handleButtonClick(ui->_2_3);
}

void MainWindow::on__2_4_clicked()
{
    handleButtonClick(ui->_2_4);
}

void MainWindow::on__2_5_clicked()
{
    handleButtonClick(ui->_2_5);
}

void MainWindow::on__2_6_clicked()
{
    handleButtonClick(ui->_2_6);
}

void MainWindow::on__2_7_clicked()
{
    handleButtonClick(ui->_2_7);
}

void MainWindow::on__2_8_clicked()
{
    handleButtonClick(ui->_2_8);
}

void MainWindow::on__3_1_clicked()
{
    handleButtonClick(ui->_3_1);
}


void MainWindow::on__3_2_clicked()
{
    handleButtonClick(ui->_3_2);
}

void MainWindow::on__3_3_clicked()
{
    handleButtonClick(ui->_3_3);
}

void MainWindow::on__3_4_clicked()
{
    handleButtonClick(ui->_3_4);
}

void MainWindow::on__3_5_clicked()
{
    handleButtonClick(ui->_3_5);
}

void MainWindow::on__3_6_clicked()
{
    handleButtonClick(ui->_3_6);
}

void MainWindow::on__3_7_clicked()
{
    handleButtonClick(ui->_3_7);
}

void MainWindow::on__3_8_clicked()
{
    handleButtonClick(ui->_3_8);
}


void MainWindow::on__4_1_clicked()
{
    handleButtonClick(ui->_4_1);
}

void MainWindow::on__4_2_clicked()
{
    handleButtonClick(ui->_4_2);
}

void MainWindow::on__4_3_clicked()
{
    handleButtonClick(ui->_4_3);
}

void MainWindow::on__4_4_clicked()
{
    handleButtonClick(ui->_4_4);
}

void MainWindow::on__4_5_clicked()
{
    handleButtonClick(ui->_4_5);
}

void MainWindow::on__4_6_clicked()
{
    handleButtonClick(ui->_4_6);
}

void MainWindow::on__4_7_clicked()
{
    handleButtonClick(ui->_4_7);
}

void MainWindow::on__4_8_clicked()
{
    handleButtonClick(ui->_4_8);
}


void MainWindow::on__5_1_clicked()
{
    handleButtonClick(ui->_5_1);
}

void MainWindow::on__5_2_clicked()
{
    handleButtonClick(ui->_5_2);
}

void MainWindow::on__5_3_clicked()
{
    handleButtonClick(ui->_5_3);
}

void MainWindow::on__5_4_clicked()
{
    handleButtonClick(ui->_5_4);
}

void MainWindow::on__5_5_clicked()
{
    handleButtonClick(ui->_5_5);
}

void MainWindow::on__5_6_clicked()
{
    handleButtonClick(ui->_5_6);
}

void MainWindow::on__5_7_clicked()
{
    handleButtonClick(ui->_5_7);
}

void MainWindow::on__5_8_clicked()
{
    handleButtonClick(ui->_5_8);
}

void MainWindow::on__6_1_clicked()
{
    handleButtonClick(ui->_6_1);
}

void MainWindow::on__6_2_clicked()
{
    handleButtonClick(ui->_6_2);
}

void MainWindow::on__6_3_clicked()
{
    handleButtonClick(ui->_6_3);
}

void MainWindow::on__6_4_clicked()
{
    handleButtonClick(ui->_6_4);
}

void MainWindow::on__6_5_clicked()
{
    handleButtonClick(ui->_6_5);
}

void MainWindow::on__6_6_clicked()
{
    handleButtonClick(ui->_6_6);
}

void MainWindow::on__6_7_clicked()
{
    handleButtonClick(ui->_6_7);
}

void MainWindow::on__6_8_clicked()
{
    handleButtonClick(ui->_6_8);
}


void MainWindow::on__7_1_clicked()
{
    handleButtonClick(ui->_7_1);
}

void MainWindow::on__7_2_clicked()
{
    handleButtonClick(ui->_7_2);
}

void MainWindow::on__7_3_clicked()
{
    handleButtonClick(ui->_7_3);
}

void MainWindow::on__7_4_clicked()
{
    handleButtonClick(ui->_7_4);
}

void MainWindow::on__7_5_clicked()
{
    handleButtonClick(ui->_7_5);
}

void MainWindow::on__7_6_clicked()
{
    handleButtonClick(ui->_7_6);
}

void MainWindow::on__7_7_clicked()
{
    handleButtonClick(ui->_7_7);
}

void MainWindow::on__7_8_clicked()
{
    handleButtonClick(ui->_7_8);
}


void MainWindow::on__8_1_clicked()
{
    handleButtonClick(ui->_8_1);
}

void MainWindow::on__8_2_clicked()
{
    handleButtonClick(ui->_8_2);
}

void MainWindow::on__8_3_clicked()
{
    handleButtonClick(ui->_8_3);
}

void MainWindow::on__8_4_clicked()
{
    handleButtonClick(ui->_8_4);
}

void MainWindow::on__8_5_clicked()
{
    handleButtonClick(ui->_8_5);
}

void MainWindow::on__8_6_clicked()
{
    handleButtonClick(ui->_8_6);
}

void MainWindow::on__8_7_clicked()
{
    handleButtonClick(ui->_8_7);
}

void MainWindow::on__8_8_clicked()
{
    handleButtonClick(ui->_8_8);
}







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                            //
//                              BOARD BUTTON FUNCTIONS END :(
//                                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
