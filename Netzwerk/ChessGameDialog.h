#ifndef CHESS_GAME_DIALOG_H
#define CHESS_GAME_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextBrowser>
#include <QRadioButton>
#include <QPushButton>
#include <QMetaObject>
#include <QMetaType>
#include <QIntValidator>
#include <stdlib.h>
#include <QTcpSocket>
#include <QNetworkInterface>
#include "ChessSocketComm.h"
#include "ChessSocketClient.h"
#include "ChessSocketServer.h"


// -- ChessGameDialog class --
// Base class for the widget which is used to start a game.

class ChessGameDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit ChessGameDialog(QWidget *parent = nullptr);
        bool isValidIPAddress(QString &ip) const;
        bool isValidPortNumber(QString &port) const;
        QString getLocalIP() const;
        ConnectionData getData() const;

    signals:
        void si_handle(const ConnectionData& data, ChessSocketServer* server, ChessSocketClient* client);
        void si_connect(const ConnectionData& data);
        void si_startUp(const ConnectionData& data);
        void si_startGame(const ConnectionData& data);
        void si_sendStart(const QString, const quint8);

    private slots:
        void onServerClicked();
        void onClientClicked();
        void onConnectClicked();
        void onStartGameClicked();
        //void toggleStartGameButton();

    private:
        QLineEdit* playerLineEdit = nullptr;
        QLineEdit* ipLineEdit = nullptr;
        QLineEdit* portLineEdit = nullptr;
        QTextBrowser* msgOutput = nullptr;
        QRadioButton* serverRadioButton = nullptr;
        QRadioButton* clientRadioButton = nullptr;
        QPushButton* connectButton = nullptr;
        QPushButton* startGameButton = nullptr;

        ConnectionData _data;
};

#endif // CHESS_GAME_DIALOG_H
