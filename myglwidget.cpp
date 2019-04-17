#include "myglwidget.h"
#include <iostream>

void MyGLWidget::fixNearFar(int nearfar) {
    if(nearfar == 0)  // near gesetzt..
       setFar(m_Near+2);

    if(nearfar == 1) // far gesetzt..
        setNear(m_Far-2);

}

void MyGLWidget::printAttr() {
    using namespace std;
    cout << "FOV= " << m_FOV << endl
         << "Angle= " << m_Angle << endl
         << "Projection Mode= ";
         m_ProjectionMode==1? cout << "Perspective\n" : cout << "Orthogonal\n";
    cout << "Near= " << m_Near << endl
         << "Far= " << m_Far << endl
         << "RotationA= " << m_RotationA << endl
         << "RotationB= " << m_RotationB << endl
         << "RotationC= " << m_RotationA << endl;
}

void MyGLWidget::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
        m_CameraPos.setY(m_CameraPos.y()+0.2);
    }
    else if(event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
        m_CameraPos.setY(m_CameraPos.y()-0.2);
    }
    else if(event->key() == Qt::Key_Up) {
        m_CameraPos.setX(m_CameraPos.x()+0.2);
    }
    else if(event->key() == Qt::Key_Down) {
        m_CameraPos.setX(m_CameraPos.x()-0.2);
    }
    else if(event->key() == Qt::Key_W) {
        m_CameraPos.setZ(m_CameraPos.z()+0.2);
    }
    else if(event->key() == Qt::Key_S) {
        m_CameraPos.setZ(m_CameraPos.z()-0.2);
    }
    else if(event->key() == Qt::Key_P) {
        std::cout << m_CameraPos.x() << ", "
                     << m_CameraPos.y() << ", "
                        << m_CameraPos.z() << std::endl;
    }
    else if (event->key() == Qt::Key_C) {
        printAttr();
    }
    else {
        QOpenGLWidget::keyPressEvent(event);
    }
}

void MyGLWidget::setFOV(int value) {
    this->m_FOV = value;
    emit fovValueChanged(value);
}
void MyGLWidget::setAngle(int value) {
    this->m_Angle = value;
    emit angleValueChanged(value);
}
void MyGLWidget::setProjectionMode(int perOrth) {
    if(perOrth == 1)
        this->m_ProjectionMode = 1; //Perspective
    if(perOrth == 2)
        this->m_ProjectionMode = 2; //Orthogonal
    emit projectionModeChanged(perOrth);
    std::cout << m_ProjectionMode << std::endl;
}
void MyGLWidget::setNear(double value) {
    this->m_Near = value;

    if(value > m_Far-2)
        emit nearFar(0);

    emit nearValueChanged(value);
}

void MyGLWidget::setFar(double value) {
    this->m_Far = value;

    if(value < m_Near+2)
        emit nearFar(1);

    emit farValueChanged(value);

}
void MyGLWidget::setRotationA(int value) {
    this->m_RotationA = value;
    emit rotationAvalueChanged(value);
}
void MyGLWidget::setRotationB(int value) {
    this->m_RotationB = value;
    emit rotationBvalueChanged(value);
}
void MyGLWidget::setRotationC(int value) {
    this->m_RotationC = value;
    emit rotationCvalueChanged(value);
}
