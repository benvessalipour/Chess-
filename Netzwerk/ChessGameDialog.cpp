#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include "ChessGameDialog.h"

ChessGameDialog::ChessGameDialog(QWidget *parent)
    : QDialog(parent)
{
    // Register ConnectionData with Qt's meta-object system
    qRegisterMetaType<ConnectionData>("ConnectionData");

    // Player Name
    QLabel* playerLabel = new QLabel("Player:");
    playerLineEdit = new QLineEdit;
    playerLineEdit->setText("Player");

    // IP Address
    QLabel* ipLabel = new QLabel("IP Address:");
    ipLineEdit = new QLineEdit;
    ipLineEdit->setText("127.0.0.1"); // Default IP

    // Port
    QLabel* portLabel = new QLabel("Port:");
    portLineEdit = new QLineEdit;
    portLineEdit->setText("8889"); // Default port

    // Server / Client Radio Buttons
    serverRadioButton = new QRadioButton("Server");
    clientRadioButton = new QRadioButton("Client");
    clientRadioButton->setChecked(true); // Default to Client

    // Connect and Start Game Buttons
    connectButton = new QPushButton("Connect");
    startGameButton = new QPushButton("Start Game");
    startGameButton->setEnabled(false); // Disabled until connected

    //msg output
    QLabel* msgLabel = new QLabel("Input/Output");
    msgOutput = new QTextBrowser;
    msgOutput->setEnabled(false); // Always disabled only for text

    // Layout for Player, IP, and Port input fields
    QVBoxLayout* inputLayout = new QVBoxLayout;
    inputLayout->addWidget(playerLabel);
    inputLayout->addWidget(playerLineEdit);
    inputLayout->addWidget(ipLabel);
    inputLayout->addWidget(ipLineEdit);
    inputLayout->addWidget(portLabel);
    inputLayout->addWidget(portLineEdit);

    // Layout for Radio Buttons
    QVBoxLayout* radioLayout = new QVBoxLayout;
    radioLayout->addWidget(serverRadioButton);
    radioLayout->addWidget(clientRadioButton);

    QVBoxLayout* outputLayout = new QVBoxLayout;
    outputLayout->addWidget(msgLabel);
    outputLayout->addWidget(msgOutput);

     // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(radioLayout);
    mainLayout->addWidget(connectButton);
    mainLayout->addWidget(startGameButton);
    mainLayout->addLayout(outputLayout);

    // Connect signals and slots
    connect(clientRadioButton, &QRadioButton::clicked, this, &ChessGameDialog::onClientClicked);
    connect(serverRadioButton, &QRadioButton::clicked, this, &ChessGameDialog::onServerClicked);
    connect(connectButton, &QPushButton::clicked, this, &ChessGameDialog::onConnectClicked);
    connect(startGameButton, &QPushButton::clicked, this, &ChessGameDialog::onStartGameClicked);
}

void ChessGameDialog::onServerClicked()
{
    ipLineEdit->setText(getLocalIP());
    ipLineEdit->setReadOnly(true);
    connectButton->setText("Launch Server");
}

void ChessGameDialog::onClientClicked()
{
    ipLineEdit->setReadOnly(false);
    connectButton->setText("Connect");
}

void ChessGameDialog::onConnectClicked()
{
    ConnectionData data;
    data.player = playerLineEdit->text();
    data.connectedIP = ipLineEdit->text();
    data.connectedPort = portLineEdit->text().toShort();

    bool isServer = serverRadioButton->isChecked();

    data.typeOfConnection = isServer ? ConnectionType::eServer : ConnectionType::eClient;

    QString tmp = QString::number(data.connectedPort);

    if (data.player.isEmpty() || data.connectedIP.isEmpty() || data.connectedPort == 0)
    {
        QMessageBox::warning(this, "Input Error", "Please fill all fields correctly.");

        if (data.player.isEmpty())
        {
            QMessageBox::warning(this, "Input Error", "Please fill in your name.");
        }
        if (data.connectedIP.isEmpty())
        {
            QMessageBox::warning(this, "Input Error", "Please fill in the host IP address you want to connect to.");
        }
        if (data.connectedPort == 0)
        {
            QMessageBox::warning(this, "Input Error", "Please fill in the host port you want to connect to.");
        }
        return;
    }

    if (!isValidIPAddress(data.connectedIP) || !isValidPortNumber(tmp))
    {
        QMessageBox::warning(this, "Type Error", "Please fill all fields correctly.");
        //msgOutput.

        if (!isValidIPAddress(data.connectedIP))
        {
            QMessageBox::warning(this, "Type Error", "Please fill in a valid IP address.");
        }
        if (!isValidPortNumber(tmp))
        {
            QMessageBox::warning(this, "Type Error", "Please fill in a valid port number.");
        }
        return;
    }

    // Connection logic (to be implemented)

    if (isServer)
    {
        emit si_startUp (data);
        QMessageBox::information(this, "Connection", "Server started on " + getLocalIP() + ":" + QString::number(data.connectedPort));
    }
    else
    {
        emit si_connect (data);
        QMessageBox::information(this, "Connection", "Connect to server at " + data.connectedIP + ":" + QString::number(data.connectedPort));
    }

    // Enable Start Game button after connection
    // Let us really check if we are connected
    startGameButton->setEnabled(true);
    _data = data;
}

void ChessGameDialog::onStartGameClicked()
{
    // Game starting logic
    QMessageBox::information(this, "Game", "Starting game...");
    emit si_startGame (getData());
    this->hide();
}

ConnectionData ChessGameDialog::getData() const
{
    return _data;
}

QString ChessGameDialog::getLocalIP() const
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
        {
            return address.toString();
        }
    }
    return QString("127.0.0.1");
}

bool ChessGameDialog::isValidIPAddress(QString &ip) const
{
    QHostAddress address;
    return address.setAddress(ip);
}

bool ChessGameDialog::isValidPortNumber(QString &port) const
{
    QIntValidator validator(0, 65535, nullptr);
    int pos = 0;
    return validator.validate(port, pos) == QValidator::Acceptable;
}
