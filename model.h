#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QString>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>

class Model : public QOpenGLWidget, QOpenGLFunctions_4_1_Core
{
public:
    Model();

    void initGL(const QString &filename);
    void drawElements();
    void finiGL();

private:
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    QOpenGLVertexArrayObject m_vao;

    unsigned m_iboCount = 0;
};

#endif // MODEL_H
