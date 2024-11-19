#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QPushButton>
#include <QDebug> // For debugging
#include <Logik/Spielfiguren.h>
#include <Logik/SpielController.h>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "Logik/Spielfiguren.h"
#include "Netzwerk/GameHandler.h"



void showGameOverDialog(char C);



QT_BEGIN_NAMESPACE

class GameHandler;

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

     Position mapButtonToPosition(QPushButton *button);
    //temporäre print funktion
     void print(const std::array<std::array<std::shared_ptr<Spielfiguren>, 8>, 8> board);

     void initializeGame();

     void highlightValidMoves(QPushButton* button);
     void clearHighlights();



signals:
    void moveMade(Position from, Position to);


private slots:


    void on_horizontalSlider_valueChanged(int value);

    void on__1_1_clicked();

    void on__1_2_clicked();

    void on_pushButton_clicked();

    void on__1_3_clicked();

    void on__1_4_clicked();

    void on__1_5_clicked();

    void on__1_6_clicked();

    void on__1_7_clicked();

    void on__1_8_clicked();

    void on__2_1_clicked();

    void on__2_2_clicked();

    void on__2_3_clicked();

    void on__2_4_clicked();

    void on__2_5_clicked();

    void on__2_6_clicked();

    void on__2_7_clicked();

    void on__2_8_clicked();

    void on__3_1_clicked();

    void on__3_2_clicked();

    void on__3_3_clicked();

    void on__3_4_clicked();

    void on__3_5_clicked();

    void on__3_6_clicked();

    void on__3_7_clicked();

    void on__3_8_clicked();

    void on__4_1_clicked();

    void on__4_2_clicked();

    void on__4_3_clicked();

    void on__4_4_clicked();

    void on__4_5_clicked();

    void on__4_6_clicked();

    void on__4_7_clicked();

    void on__4_8_clicked();

    void on__5_1_clicked();

    void on__5_2_clicked();

    void on__5_3_clicked();

    void on__5_4_clicked();

    void on__5_5_clicked();

    void on__5_6_clicked();

    void on__5_7_clicked();

    void on__5_8_clicked();

    void on__6_1_clicked();

    void on__6_2_clicked();

    void on__6_3_clicked();

    void on__6_4_clicked();

    void on__6_5_clicked();

    void on__6_6_clicked();

    void on__6_7_clicked();

    void on__6_8_clicked();

    void on__7_1_clicked();

    void on__7_2_clicked();

    void on__7_3_clicked();

    void on__7_4_clicked();

    void on__7_5_clicked();

    void on__7_6_clicked();

    void on__7_7_clicked();

    void on__7_8_clicked();

    void on__8_1_clicked();

    void on__8_2_clicked();

    void on__8_3_clicked();

    void on__8_4_clicked();

    void on__8_5_clicked();

    void on__8_6_clicked();

    void on__8_7_clicked();

    void on__8_8_clicked();

    void on_comboBox_activated(int index);

    void on_comboBox_2_activated(int index);

    void on_comboBox_3_activated(int index);





private:
    Ui::MainWindow *ui;


    QPushButton* findButtonAtPosition(const Position& position);
    QPushButton* selectedButton = nullptr;
    QList<QPushButton*> highlightedButtons;

//Board
    QPixmap Board;

//white
    QIcon blackPawn;
    QIcon blackRook;
    QIcon blackKnight;
    QIcon blackBishop;
    QIcon blackQueen;
    QIcon blackKing;

//black
    QIcon whitePawn;
    QIcon whiteRook;
    QIcon whiteKnight;
    QIcon whiteBishop;
    QIcon whiteQueen;
    QIcon whiteKing;





    // Sound
    QMediaPlayer *MoveSound;
    QMediaPlayer *CaptureSound;
    QMediaPlayer *BGMusicA;
    QMediaPlayer *BGMusicC;
    QMediaPlayer *BGMusicW;

    // Audio Output
    QAudioOutput *bgMusicAudioOutputA;
    QAudioOutput *bgMusicAudioOutputC;
    QAudioOutput *bgMusicAudioOutputW;





    // Function to handle button clicks
    void handleButtonClick(QPushButton *button);

    // Member variable to keep track of the last clicked button
    QPushButton *lastClickedButton = nullptr;

    //Logik
    SpielController controller;

    //Netzwerk
    GameHandler *_gameHandler;
};
#endif // MAINWINDOW_H
