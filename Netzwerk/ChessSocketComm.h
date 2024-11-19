#ifndef CHESSSOCKETCOMM_H
#define CHESSSOCKETCOMM_H

#include <QObject>
#include <QTcpSocket>

//----------------------------------------------------------------//
//----------------STRUCTS COMMUNICATION VOCABULARY----------------//
//----------------------------------------------------------------//

//----------------------------------------------------------------//
//------------------------- Start Game ---------------------------//

enum GROUP_NUMBER
{
    group1 = 0x01,
    group2 = 0x02,
    group3 = 0x03,
    group4 = 0x04,
    group5 = 0x05
};

enum BEGINNER
{
    server = 0x00, // Server
    client = 0x01  // Client
};

struct Beginner
{
    quint8 server = 0x00; // Server
    quint8 client = 0x01; // Client
};

struct StartGame
{
    quint8 startGame   = 0x01; // Spielbeginn
    quint8 byteLength  = 0x02; // Länge in Bytes
    quint8 beginner    = 0x00; // Beginnender
    quint8 groupNumber = 0x03; // Gruppennummer
};
Q_DECLARE_METATYPE(StartGame)

//----------------------------------------------------------------//
//---------------------- Start Game Reply ------------------------//

struct StartGameReply
{
    quint8 startGameReply    = 0x02; // Antwort auf Spielbeginn
    quint8 byteLength        = 0x02; // Länge in Bytes
    quint8 groupNumber       = 0x03; // Gruppennummer
};
Q_DECLARE_METATYPE(StartGameReply)

//----------------------------------------------------------------//
//-------------------- Additional Information --------------------//

enum CONSEQUENCES
{
    noCapture                  = 0x00, // Schlägt nicht
    capture                    = 0x01, // Schlägt
    checkMate                  = 0x02, // Schachmatt
    captureAndCheckMate        = 0x03, // Schlägt und Schachmatt
    castling                   = 0x04  // Rochade
};

struct Consequences
{
    quint8 noCapture           = 0x00; // Schlägt nicht
    quint8 capture             = 0x01; // Schlägt
    quint8 checkMate           = 0x02; // Schachmatt
    quint8 captureAndCheckMate = 0x03; // Schlägt und Schachmatt
    quint8 castling            = 0x04; // Rochade
};

enum PAWN_PROMOTION
{
    noPawnPromotion = 0x00, // Bauernumwandlung
    bishop          = 0x10, // Läufer
    knight          = 0x20, // Springer
    rook            = 0x30, // Turm
    queen           = 0x40  // Dame
};

struct PawnPromotion
{
    quint8 noPawnPromotion = 0x00; // Bauernumwandlung
    quint8 bishop          = 0x10; // Läufer
    quint8 knight          = 0x20; // Springer
    quint8 rook            = 0x30; // Turm
    quint8 queen           = 0x40; // Dame
};

struct AdditionalInformation
{
    // Differentiation between conseqeuences and pawn promotion
    // First four bits of a byte are dedicated to the consequences
    // Second four bits of a byte are dedicated for pawn promotion
    quint8 consequences; // Konsequenzen
    quint8 pawnPromotion; // Bauernumwandlung
};

//----------------------------------------------------------------//
//---------------------------- Move -----------------------------//

struct InitialColumn
{
    quint8 one   = 0x00;
    quint8 two   = 0x01;
    quint8 three = 0x02;
    quint8 four  = 0x03;
    quint8 five  = 0x04;
    quint8 six   = 0x05;
    quint8 seven = 0x06;
    quint8 eight = 0x07;
};

struct InitialRow
{
    quint8 a = 0x00;
    quint8 b = 0x01;
    quint8 c = 0x02;
    quint8 d = 0x03;
    quint8 e = 0x04;
    quint8 f = 0x05;
    quint8 g = 0x06;
    quint8 h = 0x07;
};

struct TargetColumn
{
    quint8 one   = 0x00;
    quint8 two   = 0x01;
    quint8 three = 0x02;
    quint8 four  = 0x03;
    quint8 five  = 0x04;
    quint8 six   = 0x05;
    quint8 seven = 0x06;
    quint8 eight = 0x07;
};

struct TargetRow
{
    quint8 a = 0x00;
    quint8 b = 0x01;
    quint8 c = 0x02;
    quint8 d = 0x03;
    quint8 e = 0x04;
    quint8 f = 0x05;
    quint8 g = 0x06;
    quint8 h = 0x07;
};

struct Move
{
    quint8 move                 = 0x03;
    quint8 byteLength           = 0x05;
    quint8 initialRow;
    quint8 initialColumn;
    quint8 targetRow;
    quint8 targetColumn;
    AdditionalInformation additionalInformation;
};
Q_DECLARE_METATYPE(Move)

//----------------------------------------------------------------//
//-------------------------- Move Reply --------------------------//

// Should use enum class with static_cast<STATUS>(status)
enum STATUS
{
    ok                      = 0x00,
    okCapture               = 0x01,
    okCheckMate             = 0x02,
    notOkFigureNotAvailable = 0x03,
    notOkInvalidMove        = 0x04,
    notOkNotCaptured        = 0x05,
    notOkNoTurn             = 0x06,
    notOkNotCheckMate       = 0x07
};

struct Status
{
    quint8 ok                      = 0x00;
    quint8 okCapture               = 0x01;
    quint8 okCheckMate             = 0x02;
    quint8 notOkFigureNotAvailable = 0x03;
    quint8 notOkInvalidMove        = 0x04;
    quint8 notOkNotCaptured        = 0x05;
    quint8 notOkNoTurn             = 0x06;
    quint8 notOkNotCheckMate       = 0x07;
};

struct MoveReply
{
    quint8 moveReply  = 0x04;
    quint8 byteLength = 0x01;
    STATUS status;
};
Q_DECLARE_METATYPE(MoveReply) // Do not forget to regeister Q_data_metatype

struct Commands
{
    StartGame startGame;
    StartGameReply startGameReply;
    Move move;
    MoveReply movereply;
};

enum class COMMANDS {STARTGAME = 0x00, STARTGAMEREPLY = 0x01, MOVE = 0x02, MOVEREPLY = 0x03};

// -- ConnectionType enum --

enum class ConnectionType { eServer = 0, eClient = 1};
//Q_ENUM(ConnectionType) // Enable meta-object for ConnectionType

// -- ConnectionData struct --

struct ConnectionData
{
    QString player;
    QString connectedIP;
    quint16 connectedPort = 8889;
    ConnectionType typeOfConnection = ConnectionType::eClient;
};
Q_DECLARE_METATYPE(ConnectionData)  // Declare ConnectionData as a Qt metatype

// -- Position class --
// This class is needed and given for implementation inside of the mainwindow.cpp
// It takes in the position data in the defined from by the logic developper



#endif // CHESSSOCKETCOMM_H
