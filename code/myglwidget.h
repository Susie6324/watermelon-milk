#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QVector3d>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <Lightspot.h>
#include "Particles.h"
#include "NetworkManager.h"

class MyGLWidget : public QOpenGLWidget, public QOpenGLExtraFunctions {
    Q_OBJECT

public:
    MyGLWidget(QWidget* parent = nullptr);
    ~MyGLWidget();
    void setNetworkManager(NetworkManager* manager);

protected:
    LightSpot* light;
    Particles* particles_gua, *particles_milk_dao, *particles_gua_dao, *particles_milk;
    double eye1 = 0, eye2 = 0;
    double pi = 3.1415926 / 180;
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    void handleRemoteKey(QKeyEvent* e);

private slots:
    void onMessageReceived(const QByteArray& data);

private:
    QTimer* timer;
    NetworkManager* network = nullptr;
};
#endif // MYGLWIDGET_H
