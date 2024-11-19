#include "ChessSocketServer.h"

ChessSocketServer::ChessSocketServer(quint16 port, QObject *parent)
    : QObject{parent}, _port(port), _server(new QTcpServer(this)), _socket(new QTcpSocket)
{
    connect(_server, &QTcpServer::newConnection, this, &ChessSocketServer::onReceiveNewConnection);

    connect(this, &ChessSocketServer::si_streamReady, this, &ChessSocketServer::sendStream);

    connect(this, &ChessSocketServer::si_streamSent, this, &ChessSocketServer::handleStreamSent);

    QDataStream _inputStream(&_byteArray, QIODevice::WriteOnly);
    QDataStream _outputStream(&_byteArray, QIODevice::ReadOnly);
}

void ChessSocketServer::onStartServer()
{
    if(!_server->listen(QHostAddress::Any, _port))
    {
        qDebug() << QString("Server could not start on port %1").arg(_port);
    }
    else
    {
        qDebug() << QString("Server started on port %1!").arg(_port);
    }
}

void ChessSocketServer::onReceiveNewConnection()
{
    _socket = _server->nextPendingConnection();
    connect(_socket, &QTcpSocket::readyRead, this, &ChessSocketServer::onReceiveStream);
    if (!_socket) return;
}

//----------------------------------------------------------------//
//------------METHODS FOR SENDING AND RECEIVING STREAMS-----------//
//----------------------------------------------------------------//

//------------------------- sendStream() -------------------------//

void ChessSocketServer::sendStream()
{
    std::cout << "Sending stream..." << std::endl;

    int written = _socket->write(_byteArray);
    _socket->flush();
    _socket->waitForBytesWritten();

    bool success = true ? written!=-1 : written==-1;

    qDebug() << "Stream sent: " << success;
    printStream();

    emit si_streamSent(success);
}

void ChessSocketServer::handleStreamSent(bool success)
{
    _result = success;
    _loop.quit();
}

//---------------------- onReceiveStream() ------------------------//

void ChessSocketServer::onReceiveStream()
{
    QDataStream incomingStream(_socket);
    quint8 firstByte;

    StartGame startGame;
    StartGameReply startGameReply;
    Move move;
    MoveReply moveReply;

    incomingStream >> firstByte;
    qDebug() << "This is my first byte: " << firstByte;
    switch (firstByte)
    {
        case 1:
            incomingStream >> startGame.byteLength >>
                              startGame.beginner >>
                              startGame.groupNumber;
            printMessage(startGame);
            emit si_startNewGame(startGame);
            break;

        case 2:
            incomingStream >> startGameReply.byteLength >>
                              startGameReply.groupNumber;
            printMessage(startGameReply);
            emit si_startNewGameReply(startGameReply);
            break;

        case 3:
            quint8 addInf;
            incomingStream >> move.byteLength >>
                              move.initialColumn >> move.initialRow >>
                              move.targetColumn >> move.targetRow >>
                              addInf;
            // get hex of addInf and convert to String
            // take first digit --> set the consequence to a value from the enum collection
            // take second digit --> pawnPromotion to a value from the figure collection
            bool ok;

            move.additionalInformation.consequences = QString(QString("%1").arg(addInf, 0, 16)[0]).toInt(&ok);
            move.additionalInformation.pawnPromotion = QString(QString("%1").arg(addInf, 0, 16)[1]).toInt(&ok);
            printMessage(move);
            emit si_newMove(move);
            break;

        case 4:
            incomingStream >> moveReply.moveReply >>
                              moveReply.byteLength >>
                              moveReply.status;
            printMessage(moveReply);
            emit si_newMoveReply(moveReply);
            break;
    }
}

void ChessSocketServer::handleStreamReceived()
{

}

//----------------------------------------------------------------//
//---------METHODS FOR PUSHING INFORMATION TO THE STREAM ---------//
//----------------------------------------------------------------//

bool ChessSocketServer::startSTC(QString beginner, quint8 groupNumber)
{
    StartGame startGame;
    startGame.groupNumber = groupNumber;

    if (beginner == "server" || beginner == "Server") {
        startGame.beginner = 0x00;
    } else {
        startGame.beginner = 0x01;
    }

    QDataStream inputStream(&_byteArray, QIODevice::WriteOnly);

    inputStream << startGame.startGame  <<
                   startGame.byteLength <<
                   startGame.beginner   <<
                   startGame.groupNumber;

    emit si_streamReady();
    _loop.exec();

    return _result;
}

bool ChessSocketServer::moveSTC(Position from, Position to, quint8 consequences, quint8 pawnPromotion)
{
    Move move = mapPositions(from, to);

    QDataStream inputStream(&_byteArray, QIODevice::WriteOnly);
    quint8 addInf = consequences + pawnPromotion;

    inputStream << move.move                             <<
                   move.byteLength                       <<
                   move.initialColumn << move.initialRow <<
                   move.targetColumn << move.targetRow   <<
                   addInf;

    auto addition = consequences + pawnPromotion;

    qDebug() << "Consequences and PawnPromotion in decimal: " << addInf;
    qDebug() << "Consequences and PawnPromotion in hex:     " << Qt::hex << addInf;
    qDebug() << "Consequences and PawnPromotion in int: " << addition;

    emit si_streamReady();
    _loop.exec();

    return _result;
}

bool ChessSocketServer::startReplySTC(quint8 groupNumber)
{
    StartGameReply startGameReply;

    QDataStream inputStream(&_byteArray, QIODevice::WriteOnly);

    inputStream << startGameReply.startGameReply <<
                   startGameReply.byteLength     <<
                   groupNumber;

    emit si_streamReady();
    _loop.exec();

    return _result;
}

bool ChessSocketServer::moveReplySTC(quint8 status)
{
    MoveReply moveReply;

    QDataStream inputStream(&_byteArray, QIODevice::WriteOnly);

    inputStream << moveReply.moveReply  <<
                   moveReply.byteLength <<
                   status;

    emit si_streamReady();
    _loop.exec();

    return _result;
}

StartGame ChessSocketServer::startCTS()
{
    StartGame startGame;
    return startGame;
}

Move ChessSocketServer::moveCTS()
{
    Move move;
    return move;
}

StartGameReply ChessSocketServer::startReplyCTS()
{
    StartGameReply startGameReply;
    return startGameReply;
}

MoveReply ChessSocketServer::moveReplyCTS()
{
    MoveReply moveReply;
    return moveReply;
}

//----------------------------------------------------------------//
//-----------METHODS FOR MAPPING DIFFERENT COORDINATES------------//
//----------------------------------------------------------------//

Move ChessSocketServer::mapPositions(Position from, Position to)
{
    Move move;

    // Rechne um in mein eigenes Konstrukt
    // ...
    // Steht jetzt richtig in meinen vier Variablen

    move.initialColumn = from.x;
    move.initialRow    = from.y;
    move.targetColumn  = to.x;
    move.targetRow     = to.y;

    // Hier direkte 1 to 1 implementation.

    return move;
}

void ChessSocketServer::printStream()
{
    qDebug() << "-------------------------------------------";
    qDebug() << "The test function printStream() was called!";
    qDebug() << "-------------------------------------------";

    QDataStream outputStream(&_byteArray, QIODevice::ReadOnly);

    quint8 byte1;
    quint8 byte2;
    quint8 byte3;
    quint8 byte4;
    quint8 byte5;
    quint8 byte6;
    quint8 byte7;

    outputStream >> byte1;

    switch (byte1)
    {
        case 1:
            outputStream  >> byte2 >> byte3 >> byte4;
            qDebug() << "Sent message StartGame";
            qDebug() << "Decimal:     " << byte1 << byte2 << byte3 << byte4;
            qDebug() << "Hexadecimal: " << Qt::hex << byte1 << Qt::hex << byte2 << Qt::hex << byte3 << Qt::hex << byte4;
            break;
        case 2:
            outputStream  >> byte2 >> byte3;
            qDebug() << "Sent message StartGameReply";
            qDebug() << "Decimal:     " << byte1 << byte2 << byte3;
            qDebug() << "Hexadecimal: " << Qt::hex << byte1 << Qt::hex << byte2 << Qt::hex << byte3;
            break;
        case 3:
            outputStream  >> byte2 >> byte3 >> byte4 >> byte5 >> byte6 >> byte7;
            qDebug() << "Sent message Move";
            qDebug() << "Decimal:     " << byte1 << byte2 << byte3 << byte4 << byte5 << byte6 << byte7;
            qDebug() << "Hexadecimal: " << Qt::hex << byte1<< Qt::hex << byte2 << Qt::hex << byte3 << Qt::hex << byte4 << Qt::hex << byte5 << Qt::hex << byte6 << Qt::hex <<byte7;
            break;
        case 4:
            outputStream  >> byte2 >> byte3;
            qDebug() << "Sent message MoveReply";
            qDebug() << "Decimal:     " << byte1 << byte2 << byte3 ;
            qDebug() << "Hexadecimal: " << Qt::hex << byte1 << Qt::hex << byte2 << Qt::hex << byte3;
            break;
    }

    qDebug() << "------------------------------------------";
}
