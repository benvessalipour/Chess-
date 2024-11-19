#ifndef CHESSSOCKETCLIENT_H
#define CHESSSOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTextStream>
#include <QIntValidator>
#include <stdlib.h>
#include <QtGlobal>
#include <QDebug>
#include <QEventLoop>
#include <type_traits>
#include <iostream>
#include "ChessSocketComm.h"
#include "Spielfiguren.h"

class ChessSocketClient : public QObject
{
    Q_OBJECT

    public:
        explicit ChessSocketClient(QObject *parent = nullptr);
        bool isValidIPAddress(QString &ip) const;
        bool isValidPortNumber(QString &port) const;
        void receiveBytes();
        void sendStream();
        void printStream();
        template <typename T>
        void printMessage(T command);

        bool startCTS(QString beginner, quint8 groupNumber);
        bool moveCTS(Position from, Position to, quint8 consequences, quint8 pawnPromotion);

        bool startReplyCTS(quint8 groupNumber);
        bool moveReplyCTS(quint8 status);

    signals:
        void mySignal(QTcpSocket *socket);
        void si_newCommand();
        void si_streamReady();
        void si_streamSent(bool success);

        void si_startNewGame(StartGame startGame);
        void si_startNewGameReply(StartGameReply startGameCommand);
        void si_newMove(Move move);
        void si_newMoveReply(MoveReply moveReply);

    public slots:
        bool connectToHost(ConnectionData data);
        bool disconnectFromHost();
        void receiveStreamClient();
        void customReply();

        //void onReadyRead();

    private slots:
        void handleStreamSent(bool success);

    private:

        bool          _result;
        QEventLoop      _loop;
        QByteArray _byteArray;
        quint16         _port;
        bool     _connected = false;
        QString           _ip;
        QTcpSocket   *_socket;

        quint16 getPortInput();
        QString getIPInput();

        Move mapPositions(Position from, Position to);
};


template <class T>
void ChessSocketClient::printMessage(T command)
{
    qDebug() << "--------------------------------------------";
    qDebug() << "The test function printMessage() was called!";
    qDebug() << "--------------------------------------------";

    if constexpr (std::is_same<T, StartGame>::value)
    {
        auto startGame = command;
        qDebug() << "Sent message StartGame";
        qDebug() << "Decimal:     " << startGame.startGame << startGame.byteLength << startGame.beginner << startGame.groupNumber;
        qDebug() << "Hexadecimal: " << Qt::hex << startGame.startGame << Qt::hex << startGame.byteLength << Qt::hex << startGame.beginner << Qt::hex << startGame.groupNumber;
    };

    if constexpr (std::is_same<T, StartGameReply>::value)
    {
        auto startGameReply = command;
        qDebug() << "Sent message StartGameReply";
        qDebug() << "Decimal:     " << startGameReply.startGameReply << startGameReply.byteLength << startGameReply.groupNumber;
        qDebug() << "Hexadecimal: " << Qt::hex << startGameReply.startGameReply << Qt::hex << startGameReply.byteLength << Qt::hex << startGameReply.groupNumber;
    };

    if constexpr (std::is_same<T, Move>::value)
    {
        auto move = command;
        qDebug() << "Sent message Move";
        qDebug() << "Decimal:     " << move.move << move.byteLength <<
            move.initialColumn << move.initialRow <<
            move.targetColumn << move.targetRow <<
            move.additionalInformation.consequences << move.additionalInformation.pawnPromotion;
        qDebug() << "Hexadecimal: " << Qt::hex << move.move<< Qt::hex << move.byteLength <<
            Qt::hex << move.initialColumn << Qt::hex << move.initialRow <<
            Qt::hex << move.targetColumn << Qt::hex << move.targetRow <<
            Qt::hex << move.additionalInformation.consequences << Qt::hex << move.additionalInformation.pawnPromotion;
    };

    if constexpr (std::is_same<T, MoveReply>::value)
    {
        auto moveReply = command;
        qDebug() << "Sent message MoveReply";
        qDebug() << "Decimal:     " << moveReply.moveReply << moveReply.byteLength << moveReply.status;
        qDebug() << "Hexadecimal: " << Qt::hex << moveReply.moveReply << Qt::hex << moveReply.byteLength << Qt::hex << moveReply.status;
    };

    qDebug() << "--------------------------------------------";
}

#endif // CHESSSOCKETCLIENT_H
