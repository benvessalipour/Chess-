#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <QObject>
#include "ChessSocketComm.h"
#include "ChessSocketClient.h"
#include "ChessSocketServer.h"

/// The class GameHandler is the digital interface of the network classes.
/// The instance of this class mangages the communication methods to communicate to the other participant, i.e. the server or the client.
/// GameHandler class also emits signals which contain information about received TCP information.
/// Starting a new game and a move of a piece on the chessboard is emitted by the two signals si_startNewGame and si_moveFigureTo.
class GameHandler : public QObject
{
    Q_OBJECT

public:
    GameHandler (QObject* parent = nullptr, ChessSocketClient *client = nullptr, ChessSocketServer *server = nullptr);

    /**
     * \brief Send a start new game command to the other participant.
     * Automatically sends start new game to the server if you are the client and elsewise to the client if you are the server.
     * \param beginner ENUM type which distinguishes who begins the game, e.g. is white.
     * If beginner = ConnectionType::eClient then the client plays white and the server plays black.
     * If beginner = ConnectionType::eServer then the server plays white and the client plays black.
     * \param groupNumber ENUM type which simply states which group number you are.
     * \return The notice which is true, if the message was successfully sent via the TCP connection. False if the transmission failed.
     */
    bool start(ConnectionType beginner, GROUP_NUMBER groupNumber); // Send start new game

    /**
     * \brief Send a move command to the other participant.
     * Automatically sends move command to the server if you are the client and elsewise to the client if you are the server.
     * \param from ENUM type which cointains of int x and int y and gives the position on the board, where int x is the column and int y is the row.
     * The Position from specifies where a certain figure is positioned on the chess board currently.
     * \param to ENUM type which cointains of int x and int y and gives the position on the board, where int x is the column and int y is the row.
     * The Position to specifies where the figure is supposed to be moved to on the chess board.
     * \param consequence ENUM type which holds information about what will happen during the figure piece move.
     * To know which consequences are available, see the enum definition inside of ChessSocketComm.h .
     * \param pawnPromotion ENUM type which holds information about the fiugre the pawn will be promoted to after reaching the opposit side of the board.
     * The pawn can be promoted to queen, rook, knight and rug. To see the full definition of enum pawnPromotion check in ChessSocketComm.h .
     * \return The notice which is true, if the message was successfully sent via the TCP connection. False if the transmission failed.
     */
    bool move(Position from, Position to, CONSEQUENCES consequence, PAWN_PROMOTION pawnPromotion); // Send move

signals:
    void si_startNewGame(StartGame startGame);
    void si_newMove(Move move);

    void si_startReplyError(GROUP_NUMBER groupNumber, QString errorReason);
    void si_moveReplyError(STATUS status, QString errorReason);

public slots:
    void onHandleConnection(const ConnectionData &data);

    void onStartReply(GROUP_NUMBER groupNumber);

    void onMoveReply(STATUS status);

private:
    /**
     * \brief Send a startReply command to the other participant. This function is only used internally and is therfore set private.
     * The function is called by the slot onStartReply(GROUP_NUMBER) which captures the signal incoming from the main chess logic management.
     * The main chess logic management starts a new game and simply replies when the mainwindow of a new game was started and the new game can begin.
     * Automatically sends startReply command to the server if you are the client and elsewise to the client if you are the server.
     * \param groupNumber ENUM GROUP_NUMBER type contains the groupNumber of the group.
     * \return The notice which is true, if the message was successfully sent via the TCP connection. False if the transmission failed.
     */
    bool startReply(GROUP_NUMBER groupNumber); // Send start new game reply after received request StartGame startReceived()

    /**
     * \brief Send a moveReply command to the other participant. This function is only used internally and is therfore set private.
     * It is called by the slot onMoveReply(GROUP_NUMBER) which captures the signal incoming from the main chess logic management.
     * The main chess logic management checks if a previously received move over the signal move was valid.
     * Automatically sends moveReply command to the server if you are the client and elsewise to the client if you are the server.
     * \param status ENUM STATUS type contains information about if a move was valid, a piece was captured, if it is check mate.
     * Also you can specify errors like if a figure is not available, if the move was detected as invalid, the figure can't be captured due to an invalid move,
     * it isn't one's turn and not Ok not Check Mate. To see further specification see inside of the ChessSocketComm definition.
     * \return The notice which is true, if the message was successfully sent via the TCP connection. False if the transmission failed.
     */
    bool moveReply(STATUS status); // Send move Reply after received request moveReceived()

    ConnectionType _connectionType;
    ChessSocketClient     *_client;
    ChessSocketServer     *_server;
};

#endif // GAMEHANDLER_H
