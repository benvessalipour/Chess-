#include <QApplication>

#include "GameHandler.h"
#include "ChessSocketClient.h"
#include "ChessSocketServer.h"
#include "ChessGameDialog.h"


#include <QObject>
#include <QDebug>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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

    return a.exec();
}
