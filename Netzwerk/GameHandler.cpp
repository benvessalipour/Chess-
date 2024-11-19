#include "GameHandler.h"

GameHandler::GameHandler(QObject *parent, ChessSocketClient *client, ChessSocketServer *server)
    : QObject(parent), _client(client), _server(server)
{
    // Signal das rausgesendet wird dass verbunden werden soll
    // ChessSocketClient schickt emit ein Signal zurück wenn es verbunden wurde.
    // Dieses Signal wird in der main routine mit dem Chess Dialog mit einem Slot verknüpft
    // was das Start Game Button ein oder ausschaltet, sobald ein Disconnect passiert.
    // Wenn es einen random disconnect gibt, dann sollte das Spiel einen zwischenspeicher haben
    // den aktuellen Zustand in ein file schreiben, das bei neustart wieder gelesen werden kann.
    // So könnte man auch wie bei jedem Spiel ein Resume oder Start New Game machen.
    // Omar hat nur ein QWidget, das soll dann eingeblendet werden sobald start new game gedrückt wird
    // Das Widget wird dann bei mir im ChessGameDialog instanziiert
    connect(_client, &ChessSocketClient::si_startNewGame, this, &GameHandler::si_startNewGame);
    connect(_server, &ChessSocketServer::si_startNewGame, this, &GameHandler::si_startNewGame);
    connect(_client, &ChessSocketClient::si_newMove, this, &GameHandler::si_newMove);
    connect(_server, &ChessSocketServer::si_newMove, this, &GameHandler::si_newMove);
}

void GameHandler::onHandleConnection(const ConnectionData &data)
{
    _connectionType = data.typeOfConnection;

    qInfo() << "GameHandler started";
    qInfo() << "Player: " << data.player
            << "IP: " <<  data.connectedIP
            << "Port" << data.connectedPort
            << "ConnectionType" << (data.typeOfConnection == ConnectionType::eServer ? "Server" : "Client");
}

bool GameHandler::start(ConnectionType beginner, GROUP_NUMBER groupNumber)
{
    QString Beginner = "client";
    if (beginner == ConnectionType::eServer)
    {
        Beginner = "server";
    }

    if (_connectionType == ConnectionType::eServer)
    {
        return _server->startSTC(Beginner, groupNumber);
    }
    else
    {
        return _client->startCTS(Beginner, groupNumber);
    }
}

bool GameHandler::move(Position from, Position to, CONSEQUENCES consequence, PAWN_PROMOTION pawnPromotion)
{
    if (_connectionType == ConnectionType::eServer)
    {
        return _server->moveSTC(from, to, consequence, pawnPromotion);
    }
    else
    {
        return _client->moveCTS(from, to, consequence, pawnPromotion);
    }
}

bool GameHandler::startReply(GROUP_NUMBER groupNumber)
{
    if (_connectionType == ConnectionType::eServer)
    {
        return _server->startReplySTC(groupNumber);
    }
    else
    {
        return _client->startReplyCTS(groupNumber);
    }
}

bool GameHandler::moveReply(STATUS status)
{
    if (_connectionType == ConnectionType::eServer)
    {
        return _server->moveReplySTC(status);
    }
    else
    {
        return _client->moveReplyCTS(status);
    }
}

void GameHandler::onStartReply(GROUP_NUMBER groupNumber)
{
    bool result = startReply(groupNumber);
    if (!result)
    {
        emit si_startReplyError(groupNumber, "Message sending failed");
    }
}

void GameHandler::onMoveReply(STATUS status)
{
    bool result = moveReply(status);
    if (!result)
    {
        emit si_moveReplyError(status, "Message sending failed");
    }
}
