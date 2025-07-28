
#include "myglwidget.h"
#include "NetworkManager.h"
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetworkManager* net = new NetworkManager();
    // 让玩家选择“创建主机”或“加入主机”
    QStringList options;
    options << "Local" << "Create host" << "Join host";
    bool ok = false;
    QString choice = QInputDialog::getItem(nullptr, "Choose", "Choose:", options, 0, false, &ok);

    if (!ok)
        return 0;       // 取消

    MyGLWidget* w = new MyGLWidget();
    if (choice == "Local") {
        w->setWindowTitle("Watermelon vs Milk Local");
        w->setNetworkManager(net);
        w->show();
    }
    else {
        if (choice == "Create host") {
            if (!net->startServer(6666)) {
                QMessageBox::critical(nullptr, "Error", "Server creation failed");
                return -1;
            }
            else {
                qDebug() << "Server started. Waiting for client...";
            }
        }
        else {
            QString ip = QInputDialog::getText(nullptr, "Host IP", "Host IP:", QLineEdit::Normal, "127.0.0.1", &ok);
            if (!ok || ip.isEmpty())
                return 0;

            net->connectToHost(ip, 6666);
            qDebug() << "Connecting to host at" << ip;
        }

        QObject::connect(net, &NetworkManager::connected, [&]() {
            qDebug() << "Connection established! Starting game...";
            // 连接成功 → 创建窗口并显示
            w->setWindowTitle("Watermelon vs Milk");
            w->setNetworkManager(net);
            w->show();
        });
    }
    //MyGLWidget w;
    //w.setWindowTitle("Watermelon vs Milk");
    //w.setNetworkManager(net);
    //w.show();
    return a.exec();
}
