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
//    // ���������������󶨶˿ڣ�
//    bool startServer(quint16 port = 6666);
//
//    // �ͻ������ӣ�����һ�� hello ������ݣ�
//    void connectToHost(const QString& address, quint16 port = 6666);
//
//    // �������ݸ��Է�
//    void sendMessage(const QByteArray& data);
//
//signals:
//    void messageReceived(const QByteArray& data);
//    void connected();
//    void disconnected();  // ��ʱ����ռλ
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

    // ����Ϊ��������ͻ���
    bool startServer(quint16 port);
    bool connectToHost(const QString& address, quint16 port);

    // ��������
    void sendMessage(const QByteArray& data);

signals:    // �����ź�
    void messageReceived(const QByteArray& data);
    void connected();
    void disconnected();
    void errorOccurred(const QString& errorMsg);

private slots:  // �����
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);

private:
    QTcpServer* server = nullptr;
    QTcpSocket* socket = nullptr;
};