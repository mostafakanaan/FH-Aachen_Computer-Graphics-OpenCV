#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#include <iostream>
#include <vector>
#include <QtMath>
#include <QWidget>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QKeyEvent>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include "modelloader.h"
#include "model.h"

class MyGLWidget : public QOpenGLWidget, private QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();
    void printAttr();
    void fixNearFar(int nearfar);
    void keyPressEvent(QKeyEvent *event);


private:
    int m_FOV=45;
    int m_Angle=0;
    int m_ProjectionMode = 0;
    double m_Aspect=0.0;
    double m_Near=0.0;
    double m_Far=0.0;
    int m_RotationA=0;
    int m_RotationB=0;
    int m_RotationC=0;
    float m_alpha=0.5;
    QOpenGLShaderProgram* mp_program;
    QOpenGLShaderProgram* mp_program2;
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_vao;
    GLuint m_tex;
    GLfloat m_Udiff = 0.0;
    QVector3D m_CameraPos;
    QMatrix4x4 projectionMat;

public slots:
    void setFOV(int value);
    void setAngle(int value);
    void setProjectionMode(int perOrth); //1=per/2=orth
    void setNear(double value);
    void setFar(double value);
    void setRotationA(int value);
    void setRotationB(int value);
    void setRotationC(int value);
    void setAlpha();
    void setUco();
    void updateProjectionMatrix();


signals:
    void fovValueChanged(int newFOVvalue);
    void angleValueChanged(int newAngleValue);
    void projectionModeChanged(int perOrth); //1=per/2=orth
    void nearValueChanged(double newNearValue);
    void farValueChanged(double newFarValue);
    void rotationAvalueChanged(double newRotationAvalue);
    void rotationBvalueChanged(double newRotationBvalue);
    void rotationCvalueChanged(double newRotationCvalue);
    void nearFar(int nearfar); //0=near/1=far

    // QOpenGLWidget interface
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void finalizeGL();

};

#endif // MYGLWIDGET_H
