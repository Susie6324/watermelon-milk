#include "Particles.h"
const int MAX_PARTICLES_SIZE = 1000000;
Particles::Particles(const char* vertex1Path, const char* fragment1Path, const char* vertex2Path, const char* fragment2Path)
{
    initializeOpenGLFunctions();
    VBO.create();
    VBO.bind();
    VBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    VBO.allocate(sizeof(Particle) * MAX_PARTICLES_SIZE);

    VBOBuffer.create();
    VBOBuffer.bind();
    VBOBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    VBOBuffer.allocate(sizeof(Particle) * MAX_PARTICLES_SIZE);

    glGenQueries(1, &particleQuery);

    VAO.create();
    VAO.bind();

    updateProgram.create();
    updateProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vertex1Path);
    updateProgram.addShaderFromSourceFile(QOpenGLShader::Geometry, fragment1Path);

    const char* feedbackVar[5] = { "position","velocity","size","life","color" };
    glTransformFeedbackVaryings(updateProgram.programId(), 5, feedbackVar, GL_INTERLEAVED_ATTRIBS);
    updateProgram.link();

    VBO.bind();
    updateProgram.enableAttributeArray(0);
    updateProgram.enableAttributeArray(1);
    updateProgram.enableAttributeArray(2);
    updateProgram.enableAttributeArray(3);
    updateProgram.enableAttributeArray(4);

    renderProgram.create();
    renderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vertex2Path);
    renderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fragment2Path);
    renderProgram.link();

    timeOfLastFrame = QTime::currentTime();
    VAO.bind();
    VBO.bind();
    lightProgram.create();
    lightProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "basiclight.vs");
    lightProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "basiclight.fs");
    lightProgram.link();
    VAO.release();
    VBO.release();
    glEnable(GL_PROGRAM_POINT_SIZE);
}
void Particles::use(QMatrix4x4 VP, QVector3D color, QVector3D back)
{
    createParticles(color);
    updateParticles(back);
    renderParticles(VP);
}
void Particles::createParticles(QVector3D color)
{
    VAO.bind();
    VBO.bind();
    QVector<Particle> createBuffer;
    for (int i = 0; i < numOfPerFrame; i++) {
        Particle particle;
        particle.position = QVector3D(0, 0, 0);
        particle.velocity = QVector3D((rand() % 1000 / 1000.0 - 0.5) * 0.1, 0.1, (rand() % 1000 / 1000.0 - 0.5) * 0.1);
        particle.size = defaultSize;
        particle.life = 0;
        particle.color = QVector4D(color, 1);
        createBuffer << particle;
    }
    VBO.write(currentNumOfParticles * sizeof(Particle), createBuffer.data(), createBuffer.size() * sizeof(Particle));
    currentNumOfParticles += createBuffer.size();

    VAO.release();
    VBO.release();
}

void Particles::updateParticles(QVector3D back)
{
    float durationSecond = timeOfLastFrame.msecsTo(QTime::currentTime()) / 1000.0;
    timeOfLastFrame = QTime::currentTime();
    VAO.bind();
    VBO.bind();
    
    updateProgram.bind();
    updateProgram.setUniformValue("lifetime", lifetime);
    updateProgram.setUniformValue("duration", durationSecond);
    updateProgram.setUniformValue("back", back);
    updateProgram.setAttributeBuffer(0, GL_FLOAT, offsetof(Particle, position), 3, sizeof(Particle));
    updateProgram.setAttributeBuffer(1, GL_FLOAT, offsetof(Particle, velocity), 3, sizeof(Particle));
    updateProgram.setAttributeBuffer(2, GL_FLOAT, offsetof(Particle, size), 1, sizeof(Particle));
    updateProgram.setAttributeBuffer(3, GL_FLOAT, offsetof(Particle, life), 1, sizeof(Particle));
    updateProgram.setAttributeBuffer(4, GL_FLOAT, offsetof(Particle, color), 4, sizeof(Particle));

    glEnable(GL_RASTERIZER_DISCARD);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, VBOBuffer.bufferId());
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, particleQuery);
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, currentNumOfParticles);
    glEndTransformFeedback();
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    glGetQueryObjectuiv(particleQuery, GL_QUERY_RESULT, &currentNumOfParticles);
    glDisable(GL_RASTERIZER_DISCARD);
    qSwap(VBO, VBOBuffer);
    updateProgram.release();
    VAO.release();
    VBO.release();
}

void Particles::renderParticles(QMatrix4x4 VP)
{
    renderProgram.bind();
    VAO.bind();
    VBO.bind();
    renderProgram.setUniformValue("VP", VP);
  
    glDrawArrays(GL_POINTS, 0, currentNumOfParticles);
    VAO.release();
    VBO.release();
    renderProgram.release();
}
