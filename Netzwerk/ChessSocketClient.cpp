#include "ChessSocketClient.h"

ChessSocketClient::ChessSocketClient(QObject *parent)
    : QObject{parent}, _socket(new QTcpSocket(this))
{
    //QObject::connect(&_socket, &QTcpSocket::readyRead, this, &ChessSocketClient::onReadyRead);
    //connect(this, &QTcpSocket::readyRead, this, &ChessSocketClient::receiveBytes);
    //connect(_socket, &QTcpSocket::readyRead, this, &ChessSocketClient::receiveBytes);
    //connect(_socket, &QTcpSocket::readyRead, this, &ChessSocketClient::receiveBytes);
    connect(_socket, &QTcpSocket::readyRead, this, &ChessSocketClient::receiveStreamClient);
    connect(this, &ChessSocketClient::si_streamReady, this, &ChessSocketClient::sendStream);
    connect(this, &ChessSocketClient::si_streamSent, this, &ChessSocketClient::handleStreamSent);
    connect(this, &ChessSocketClient::si_newMove, this, &ChessSocketClient::customReply);

    //connect(this, &ChessSocketClient::si_newCommand, this, &ChessSocketClient::)
}

void ChessSocketClient::customReply()
{
    quint8 groupNumber = 0x05;
    startReplyCTS(groupNumber);
}

bool ChessSocketClient::connectToHost(ConnectionData data)
{
    if (data.typeOfConnection == ConnectionType::eServer) // If server do not start client
    {
        return false;
    }

    if (_connected)
    {
        qDebug() << "Cannot connect to host since you are already connected!";
        return false;
    }

    _ip = data.connectedIP;
    _port = data.connectedPort;

    _socket->connectToHost(data.connectedIP, data.connectedPort);

    std::cout << "connectToHost socket: " << _socket << std::endl;


    if(!_socket->waitForConnected(1000))
    {
        qDebug() << "Connection attempt failed!";
        return false;
    }
    else
    {
        qDebug() << "Connected!";
        _connected = true;
        return true;
    }
}

void ChessSocketClient::sendStream()
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

void ChessSocketClient::handleStreamSent(bool success)
{
    _result = success;
    _loop.quit();
}

void ChessSocketClient::receiveBytes()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QDataStream stream(socket);

    std::cout << "receiveBytes socket: " << socket << std::endl;

    quint8 byte1;
    quint8 byte2;
    quint8 byte3;
    quint8 byte4;

    stream >> byte1 >> byte2 >> byte3 >> byte4;

    std::cout << "Hex value Client: " << byte1 << std::endl;
    std::cout << "Hex value Client: " << byte2 << std::endl;
    std::cout << "Hex value Client: " << byte3 << std::endl;
    std::cout << "Hex value Client: " << byte4 << std::endl;

    std::cout << "Hex value Client: 0x" << std::hex << static_cast<int>(byte1) << std::endl;
    std::cout << "Hex value Client: 0x" << std::hex << static_cast<int>(byte2) << std::endl;
    std::cout << "Hex value Client: 0x" << std::hex << static_cast<int>(byte3) << std::endl;
    std::cout << "Hex value Client: 0x" << std::hex << static_cast<int>(byte4) << std::endl;

}

void ChessSocketClient::receiveStreamClient()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QDataStream incomingStream(socket);
    quint8 firstByte;

    qDebug() << "Runde 1";

    StartGame startGame;
    StartGameReply startGameReply;
    Move move;
    MoveReply moveReply;

    incomingStream >> firstByte;
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
        incomingStream >> moveReply.byteLength >>
                          moveReply.status;
        printMessage(moveReply);
        emit si_newMoveReply(moveReply);
        break;
    }
}

bool ChessSocketClient::startCTS(QString beginner, quint8 groupNumber)
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

bool ChessSocketClient::moveCTS(Position from, Position to, quint8 consequences, quint8 pawnPromotion)
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

bool ChessSocketClient::startReplyCTS(quint8 groupNumber)
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

bool ChessSocketClient::moveReplyCTS(quint8 status)
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

bool ChessSocketClient::disconnectFromHost()
{
    if (!_connected)
    {
        qDebug() << "Cannot disconnect from host since you are not connected!";
        return false;
    }

    _socket->disconnectFromHost();
    qDebug() << "Successfully disconnected from host";
    return true;
}


// Manually read input values via the command line

quint16 ChessSocketClient::getPortInput()
{
    QTextStream userInput(stdin);
    QTextStream userOutput(stdout);

    userOutput << "Please input a port number: ";

    auto content = userInput.readLine();
    bool ok = isValidIPAddress(content);
    auto result = content.toUShort(&ok);
    if (ok)
    {
        quint16 port = static_cast<quint16>(result);
        return port;
    }
    else
    {
        userOutput << "The entered port is not a port number. Please try again.";
        return getPortInput();
    }
}

QString ChessSocketClient::getIPInput()
{
    QTextStream userInput(stdin);
    QTextStream userOutput(stdout);

    userOutput << "Please input an IP address: ";

    auto ip = userInput.readLine();
    bool ok = isValidIPAddress(ip);
    if (ok)
    {
        return ip;
    }
    else
    {
        userOutput << "The entered IP is not a address. Please try again.";
        return getIPInput();
    }
}

// Check for valid IP address and valid port

bool ChessSocketClient::isValidIPAddress(QString &ip) const
{
    QHostAddress address;
    return address.setAddress(ip);
}

bool ChessSocketClient::isValidPortNumber(QString &port) const
{
    QIntValidator validator(0, 65535, nullptr);
    int pos = 0;
    return validator.validate(port, pos) == QValidator::Acceptable;
}

Move ChessSocketClient::mapPositions(Position from, Position to)
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

void ChessSocketClient::printStream()
{
    qDebug() << "------------------------------------------";
    qDebug() << "The test function printStream() was called!";
    qDebug() << "------------------------------------------";

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

/*         Position from = {5,4};
Position to = {6,3};
bool done = moveCTS(from, to, 0x04, 0x20);
qDebug() << "Sent?????? " << done;*/
