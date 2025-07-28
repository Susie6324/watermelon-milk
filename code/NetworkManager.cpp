//#include "NetworkManager.h"
//#include <QDebug>
//
//NetworkManager::NetworkManager(QObject* parent)
//    : QObject(parent)
//{
//    udpSocket = new QUdpSocket(this);
//    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
//}
//
//bool NetworkManager::startServer(quint16 port)
//{
//    isServer = true;
//    bool success = udpSocket->bind(QHostAddress::AnyIPv4, port);
//    if (!success) {
//        qWarning() << "UDP bind failed!";
//    }
//    else {
//        qDebug() << "UDP server started on port" << port;
//    }
//    return success;
//}
//
//void NetworkManager::connectToHost(const QString& address, quint16 port)
//{
//    peerAddress = QHostAddress(address);
//    peerPort = port;
//
//    // UDP 无连接：发送一条 hello 报文告知对方我存在
//    sendMessage("HELLO");
//    qDebug() << "UDP client mode. Sent HELLO to" << address << ":" << port;
//}
//
//void NetworkManager::sendMessage(const QByteArray& data)
//{
//    if (peerAddress.isNull() || peerPort == 0) {
//        qWarning() << "UDP peer not set. Message not sent.";
//        return;
//    }
//    udpSocket->writeDatagram(data, peerAddress, peerPort);
//}
//
//void NetworkManager::onReadyRead()
//{
//    while (udpSocket->hasPendingDatagrams()) {
//        QByteArray data;
//        data.resize(int(udpSocket->pendingDatagramSize()));
//        QHostAddress sender;
//        quint16 senderPort;
//
//        udpSocket->readDatagram(data.data(), data.size(), &sender, &senderPort);
//
//        // 自动记录对方 IP+端口（只记录第一次）
//        if (peerAddress.isNull()) {
//            peerAddress = sender;
//            peerPort = senderPort;
//            emit connected();
//            qDebug() << "Connected to peer via UDP:" << sender.toString() << ":" << senderPort;
//        }
//
//        // 触发输入逻辑
//        emit messageReceived(data);
//    }
//}


#include "NetworkManager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent)
{
}

bool NetworkManager::startServer(quint16 port)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    if (!server->listen(QHostAddress::Any, port)) {
        emit errorOccurred("Server failed to start: " + server->errorString());
        return false;
    }

    qDebug() << "Server started on port" << port;
    return true;
}

bool NetworkManager::connectToHost(const QString& address, quint16 port)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(socket, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(onError(QAbstractSocket::SocketError)));

    socket->connectToHost(address, port);
    qDebug() << "connectToHost:" << address << ":" << port;
    return true;
}

void NetworkManager::onNewConnection()
{
    socket = server->nextPendingConnection();
    socket->write("Hello from server");
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));            // 关联信号与槽
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    emit connected();       // 触发对应的信号
    qDebug() << "Client connected.";
}

void NetworkManager::onReadyRead()
{
    QByteArray data = socket->readAll();
    qDebug() << "Client revc:" << data;
    emit messageReceived(data);
}

void NetworkManager::sendMessage(const QByteArray& data)
{
    if (socket && socket->isOpen()) {
        socket->write(data);
        socket->flush();
    }
}

void NetworkManager::onDisconnected()
{
    emit disconnected();
}

void NetworkManager::onError(QAbstractSocket::SocketError)
{
    if (socket) {
        emit errorOccurred(socket->errorString());
    }
}
