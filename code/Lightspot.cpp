#include "Lightspot.h"
#include <QVector3D>
#include <QtWidgets>
#include <QtOpenGL/qglpixelbuffer.h>
LightSpot::LightSpot() :
    VBO(QOpenGLBuffer::VertexBuffer)
    , EBO(QOpenGLBuffer::IndexBuffer)
{
    lightxpos = 0.0f;
    lightypos = 400.0f;
    lightzpos = 150.0f;
    up = false;
    down = false;
}

LightSpot::~LightSpot()
{
}


void LightSpot::init(const char* vertex1Path, const char* fragment1Path, const char* vertex2Path, const char* fragment2Path)
{
    initializeOpenGLFunctions();

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    VAO1.create();
    VAO1.bind();
    VBO.create();
    VBO.bind();
    VBO.allocate(vertices, sizeof(vertices));
    shaderProgram1.create();
    shaderProgram1.addShaderFromSourceFile(QOpenGLShader::Vertex, vertex1Path);
    shaderProgram1.addShaderFromSourceFile(QOpenGLShader::Fragment, fragment1Path);
    shaderProgram1.link();
    shaderProgram1.enableAttributeArray(0);
    shaderProgram1.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));       //将VBO和EBO的id，以及解析格式存储到VAO中
    VAO1.release();
    VBO.release();

    VAO2.create();
    VAO2.bind();
    VBO.create();
    VBO.bind();
    shaderProgram2.create();
    shaderProgram2.addShaderFromSourceFile(QOpenGLShader::Vertex, vertex2Path);
    shaderProgram2.addShaderFromSourceFile(QOpenGLShader::Fragment, fragment2Path);
    shaderProgram2.link();
    VAO2.release();
    VBO.release();
}

void LightSpot::use(QMatrix4x4& model, QMatrix4x4& projection, QVector3D viewPos, bool start) {
    static bool game_start = 0;
    if (start && !game_start) {
        up = true;
        game_start = 1;
    }
    if (lightypos >= 500) {
        up = false;
        down = true;
    }
    if (lightypos < 160) {
        up = false;
        down = false;
    }
    if (up) {
        lightxpos += 0.075 * 4;
        lightypos += 0.1 * 4;
        lightzpos -= 0.075 * 4;
    }
    if (down) {
        lightypos -= 0.3 * 2;
        //lightypos -= 0.4;
    }
    shaderProgram1.bind();
    QMatrix4x4 view;
    view.translate(lightxpos, lightypos, lightzpos);
    view.scale(10);
    shaderProgram1.setUniformValue("model", model);
    shaderProgram1.setUniformValue("projection", projection);
    shaderProgram1.setUniformValue("view", view);
    QOpenGLVertexArrayObject::Binder bind(&VAO1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    shaderProgram2.bind();
    shaderProgram2.setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
    shaderProgram2.setUniformValue("light.position", QVector3D(lightxpos, lightypos, lightzpos));
    shaderProgram2.setUniformValue("light.direction", QVector3D(0.0f, -1.0f, 0.0f));
    shaderProgram2.setUniformValue("light.constant", 1.0f);
    shaderProgram2.setUniformValue("light.linear", 0.09f);
    shaderProgram2.setUniformValue("light.quadratic", 0.032f);
    shaderProgram2.setUniformValue("viewPos", viewPos);

}
void LightSpot::release()
{
    shaderProgram1.release();
    shaderProgram2.release();
}
