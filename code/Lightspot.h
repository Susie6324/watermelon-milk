#ifndef LIGHTSPOT_H
#define LIGHTSPOT_H
#include<cstdio>
#include <GL/glew.h>
#include<cstring>
#include<iostream>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QtOpenGL/qglfunctions.h>
class LightSpot:public QOpenGLFunctions{
public:
	LightSpot();
	~LightSpot();
	void init(const char*, const char*, const char*, const char*);
	void use(QMatrix4x4&, QMatrix4x4&, QVector3D, bool start);
	void release();
	QOpenGLShaderProgram shaderProgram1, shaderProgram2;
	QOpenGLVertexArrayObject VAO1, VAO2;
	QOpenGLBuffer VBO;
	QOpenGLBuffer EBO;
	float lightxpos, lightypos, lightzpos;
	bool up, down;
};
#endif