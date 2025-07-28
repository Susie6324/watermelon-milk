//#pragma once
//#include <QObject>
//#include <QUdpSocket>
//#include <QHostAddress>
//
//class NetworkManager : public QObject
//{
//    Q_OBJECT
//public:
//    explicit NetworkManager(QObject* parent = nullptr);
//
//    // 启动服务器（即绑定端口）
//    bool startServer(quint16 port = 6666);
//
//    // 客户端连接（发送一次 hello 表明身份）
//    void connectToHost(const QString& address, quint16 port = 6666);
//
//    // 发送数据给对方
//    void sendMessage(const QByteArray& data);
//
//signals:
//    void messageReceived(const QByteArray& data);
//    void connected();
//    void disconnected();  // 暂时保留占位
//
//private slots:
//    void onReadyRead();
//
//private:
//    QUdpSocket* udpSocket = nullptr;
//    QHostAddress peerAddress;
//    quint16 peerPort = 0;
//    bool isServer = false;
//};


#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject* parent = nullptr);

    // 启动为服务器或客户端
    bool startServer(quint16 port);
    bool connectToHost(const QString& address, quint16 port);

    // 发送数据
    void sendMessage(const QByteArray& data);

signals:    // 定义信号
    void messageReceived(const QByteArray& data);
    void connected();
    void disconnected();
    void errorOccurred(const QString& errorMsg);

private slots:  // 定义槽
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);

private:
    QTcpServer* server = nullptr;
    QTcpSocket* socket = nullptr;
};