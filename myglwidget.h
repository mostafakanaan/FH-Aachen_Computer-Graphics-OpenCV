#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#include <QWidget>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QVector3D>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShader>

class MyGLWidget : public QOpenGLWidget, private QOpenGLFunctions_3_3_Core
{
public:
    MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
        setFocusPolicy(Qt::StrongFocus);
        m_CameraPos.setX(0.0); m_CameraPos.setY(0.0); m_CameraPos.setZ(0.0);
    }
    void fixNearFar(int nearfar);
    void keyPressEvent(QKeyEvent *event);
    QVector3D m_CameraPos;
    void printAttr();


private:
    Q_OBJECT
    int m_FOV=0;
    int m_Angle=0;
    int m_ProjectionMode = 0;
    double m_Near=0.0;
    double m_Far=0.0;
    int m_RotationA=0;
    int m_RotationB=0;
    int m_RotationC=0;
    float m_alpha=0.5;
    QOpenGLShaderProgram* mp_program;
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_tex;
    GLfloat m_Udiff = 0.0;

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

};

#endif // MYGLWIDGET_H
