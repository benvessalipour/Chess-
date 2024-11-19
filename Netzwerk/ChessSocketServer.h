#ifndef CHESSSOCKETSERVER_H
#define CHESSSOCKETSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTextStream>
#include <QEventLoop>
#include <QNetworkInterface>
#include "ChessSocketComm.h"
#include <iostream>
#include "Spielfiguren.h"


class ChessSocketServer : public QObject
{
    Q_OBJECT

public:
    explicit ChessSocketServer(quint16 port = 8889,
                               QObject *parent = nullptr);


    void sendStream();

    void printStream();

    // -- Send from the server STC = server to client --
    // Outgoing commands from the Server to the Client
    // return true if sending was successfull
    // return false if sending failed

    bool startSTC(QString beginner, quint8 groupNumber);
    bool moveSTC(Position from, Position to, quint8 consequences, quint8 pawnPromotion);

    bool startReplySTC(quint8 groupNumber);
    bool moveReplySTC(quint8 status);

    template <typename T>
    void printMessage (T command);

public slots:
    void onStartServer();
    void onReceiveNewConnection();
    void onReceiveStream();

    // -- Receive at the server CTS = client to server --
    // Incoming commands from the Client
    // returns the respective structs which hold the information
    // that was previously received over the stream


    StartGame startCTS(); // Client: please start new Game!
    Move moveCTS(); // Client: please move figure

    StartGameReply startReplyCTS(); // Client reply after Server request new Game
    MoveReply moveReplyCTS(); // Client reply after Server request move to position

private slots:
    void handleStreamSent(bool success);
    void handleStreamReceived();

signals:
    void si_streamReady();
    void si_streamSent(bool success);
    void si_serverStarted();

    void si_startNewGame(StartGame startGame);
    void si_startNewGameReply(StartGameReply startGameCommand);
    void si_newMove(Move move);
    void si_newMoveReply(MoveReply moveReply);

private:

    Move mapPositions(Position from, Position to);

    bool               _result;
    QEventLoop           _loop;
    QByteArray      _byteArray;
    QByteArray   _receiveArray;
    quint16              _port;
    QTcpServer        *_server;
    QTcpSocket        *_socket;
    QDataStream  *_inputStream;
    QDataStream *_outputStream;
};

template <class T>
void ChessSocketServer::printMessage(T command)
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

#endif // CHESSSOCKETSERVER_H
