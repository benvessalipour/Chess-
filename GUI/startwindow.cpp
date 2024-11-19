#include "startwindow.h"
#include "ui_startwindow.h"
#include "mainwindow.h"
#include "/Users/Git repository/CPP_SS2024_G03/schach/Netzwerk/ChessGameDialog.h"
#include "/Users/Git repository/CPP_SS2024_G03/schach/Netzwerk/GameHandler.h"

StartWindow::StartWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    ui->pushButton_3->setVisible(false);
    ui->pushButton_4->setVisible(false);
    ui->label_2->setVisible(false);

}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_pushButton_clicked()
{
    ui->label->setText( "Welcome " + ui->lineEdit->text());
    ui->pushButton_3->setVisible(true);
    ui->pushButton_4->setVisible(true);
    ui->label_2->setVisible(true);
    ui->label_2->setText("Start Game as the great and magnificent " + ui->lineEdit->text());
}


void StartWindow::on_pushButton_2_clicked()
{
    ui->label->setText("Enter your name");
    ui->lineEdit->clear();
}


void StartWindow::on_pushButton_3_clicked()
{
    MainWindow *main = new MainWindow(this); // Create an instance of MainWindow (the game window)
    main->show(); // Show the game window
    this->hide(); // Hide the start menu
}


void StartWindow::on_pushButton_4_clicked()
{
    ui->pushButton_3->setVisible(false);
    ui->pushButton_4->setVisible(false);
    ui->label_2->setVisible(false);
}

void StartWindow::on_pushButton_5_clicked()
{
    ChessGameDialog dlg;
    dlg.setWindowTitle("Chess Game");

    ChessSocketClient client;
    ChessSocketServer server;
    GameHandler handler;

    QObject::connect (&dlg, &ChessGameDialog::si_handle, &handler, &GameHandler::onHandleConnection);
    QObject::connect (&dlg, &ChessGameDialog::si_connect, &client, &ChessSocketClient::connectToHost);
    QObject::connect (&dlg, &ChessGameDialog::si_startUp, &server, &ChessSocketServer::onStartServer);
    QObject::connect (&dlg, &ChessGameDialog::si_sendStart, &server, &ChessSocketServer::startSTC);

    dlg.show();

    dlg.exec();
}

