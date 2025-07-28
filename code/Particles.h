#ifndef PARTICLES_H
#define PARTICLES_H
#include <QOpenGLWidget>
#include <QWidget>
#include <QVector3D>
#include <QVector4D>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include <QTime>
struct Particle {
	QVector3D position;
	QVector3D velocity;
	float size;
	float life;
	QVector4D color;
};
class Particles:public QOpenGLExtraFunctions
{
public:
	Particles(const char*, const char*, const char*, const char*);
	void createParticles(QVector3D);
	void updateParticles(QVector3D);
	void renderParticles(QMatrix4x4);
	void use(QMatrix4x4, QVector3D, QVector3D);
	int numOfPerFrame = 100;
	float lifetime = 1;
	double defaultSize = 1;
private:
	QOpenGLShaderProgram lightProgram;
	QOpenGLShaderProgram updateProgram;
	QOpenGLShaderProgram renderProgram;
	QOpenGLVertexArrayObject VAO;

	QOpenGLBuffer VBO;
	QOpenGLBuffer VBOBuffer;

	GLuint particleQuery;
	GLuint currentNumOfParticles = 0;
	QTime timeOfLastFrame;
};
#endif
