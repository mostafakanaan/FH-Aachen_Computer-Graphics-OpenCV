#include "myglwidget.h"
#include <iostream>
#include <vector>

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

void MyGLWidget::initializeGL() {
    bool success = initializeOpenGLFunctions();
    Q_ASSERT(success); Q_UNUSED(success);
    glClearColor(0.169f, 0.169f, 0.169f, 0.5f); //Dunkelgrau

    // INITIALISIERUNG
    // Enable and specify vertex attribute 0 (2 floats )
    // void* cast is necessary for legacy reasons

    // sizeof (Vertex) already evaluates to correct stride

    struct Vertex {
        GLfloat position[2];
        GLfloat color[3];
        GLfloat texturePos[2];
    };

    Vertex arr[] = {
        {{-0.5, -0.5},{1.0f, 0.0f, 0.0},{float(0.2)+m_Udiff,0.2}},
        {{0.5, -0.5},{0.0f, 1.0f, 0.0},{float(0.8)+m_Udiff,0.2}},
        {{0.0, 0.5},{0.0f, 0.0f, 1.0},{float(0.5)+m_Udiff,0.8}}
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // create buffer object
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // copy data
    glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr , GL_STATIC_DRAW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // define helper for offsetof that does the void* cast
    #define OFS(s, a) reinterpret_cast<void* const>(offsetof(s, a))

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, sizeof(Vertex), OFS(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFS(Vertex, texturePos));

#undef OFS

    mp_program = new QOpenGLShaderProgram();

    // addShader() compiles and attaches shader stages for linking
    mp_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/default.vert");
    mp_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/default.frag");
    // link attached stages
    mp_program->link();
    // abort program if any of these steps failed
    Q_ASSERT(mp_program->isLinked());


    QImage texImg;
    texImg.load(":/sample_texture.jpg");
    Q_ASSERT(!texImg.isNull());


    // Create texture object
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    // fill with pixel data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texImg.width(), texImg.height(),
                 0, GL_BGRA, GL_UNSIGNED_BYTE, texImg.bits());
    // set filtering ( interpolation ) options
    // without these commands, _sampling will return black_
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void MyGLWidget::resizeGL(int w, int h) {

}

void MyGLWidget::paintGL() {
    // erase old pixels
    glClear(GL_COLOR_BUFFER_BIT);
    // bind Resources
    glBindVertexArray(m_vao);
    mp_program->bind();


    mp_program->setUniformValue(0, m_alpha);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    // set wrap mode to "clamp to edge"
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // write unit indices to uniforms
    mp_program->setUniformValue(7, 0);

    // starting at vertex 0, render 3 vertices (=> 1 triangle)
    glDrawArrays(GL_TRIANGLES, 0, 3);
    update();
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

void MyGLWidget::setAlpha() {
    this->m_alpha = (float) (m_Angle) / 360.0;
}

void MyGLWidget::setUco() {
    this->m_Udiff = (float) (m_RotationA/100.0);
    initializeGL();
}
