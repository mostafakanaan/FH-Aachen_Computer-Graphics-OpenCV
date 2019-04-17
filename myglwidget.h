#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#include <QWidget>
#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QVector3D>

class MyGLWidget : public QOpenGLWidget
{
public:
    MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) { setFocusPolicy(Qt::StrongFocus);
                                                        m_CameraPos.setX(0.0);
                                                        m_CameraPos.setY(0.0);
                                                        m_CameraPos.setZ(0.0);}
    void fixNearFar();
    void resetAttr();
    void keyPressEvent(QKeyEvent *event);
    QVector3D m_CameraPos;

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

public slots:
    void setFOV(int value);
    void setAngle(int value);
    void setProjectionMode(int perOrth); //1=per/2=orth
    void setNear(double value);
    void setFar(double value);
    void setRotationA(int value);
    void setRotationB(int value);
    void setRotationC(int value);


signals:
    void fovValueChanged(int newFOVvalue);
    void angleValueChanged(int newAngleValue);
    void projectionModeChanged(int perOrth); //1=per/2=orth
    void nearValueChanged(double newNearValue);
    void farValueChanged(double newFarValue);
    void rotationAvalueChanged(double newRotationAvalue);
    void rotationBvalueChanged(double newRotationBvalue);
    void rotationCvalueChanged(double newRotationCvalue);
    void nearFar();
};

#endif // MYGLWIDGET_H
