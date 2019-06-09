#include "myglwidget.h"


MyGLWidget::MyGLWidget(QWidget *parent): QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
}

MyGLWidget::~MyGLWidget(){
    finalizeGL();
}

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
         << "RotationC= " << m_RotationC << endl;
}

void MyGLWidget::initializeGL() {
    bool success = initializeOpenGLFunctions();
    Q_ASSERT(success); Q_UNUSED(success);
    glClearColor(0.169f, 0.169f, 0.169f, 0.5f); //Dunkelgrau

    m_Aspect = this->width()/this->height();
    projectionMat.perspective(m_Angle, m_Aspect, m_Near, m_Far);

    struct Vertex {
        GLfloat position[2];
        GLfloat color[3];
        GLfloat texturePos[2];
    };

    Vertex arr[] = {
        {{-0.6, -0.4},{1.0f, 0.0f, 0.0},{0.1,0.1}},         // 0
        {{0.2, -0.4},{1.0f, 0.0f, 0.0},{0.6,0.1}},          // 1
        {{-0.2, 0.4},{0.0f, 1.0f, 0.0},{0.4,0.8}},  // 2
        {{0.6, 0.4},{0.0f, 0.0f, 1.0},{0.9,0.8}}    // 3
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    GLuint indices[] = {
        0, 1, 2,
        3, 2, 1
    };

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // create buffer object
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // copy data
    glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr , GL_STATIC_DRAW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
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
    mp_program2 = new QOpenGLShaderProgram();

    // addShader() compiles and attaches shader stages for linking
    mp_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/triangle1.vert");
    mp_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/triangle1.frag");
    mp_program2->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/triangle2.vert");
    mp_program2->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/triangle2.frag");
    mp_program->link();
    mp_program2->link();

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
    m_Aspect = (double)w/(double)h;
    std::cout << "W= " << w << ", H= " << h << "\nRatio: " << m_Aspect << std::endl;
    updateProjectionMatrix();
}

void MyGLWidget::paintGL() {
    // erase old pixels
    glClear(GL_COLOR_BUFFER_BIT);

    // bind Resources
    glBindVertexArray(m_vao);
    mp_program->bind();


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_tex);

    // set wrap mode to "clamp to edge"
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // rotation axis (= Y-axis)
    QVector3D rotAxis(0, 1, 0);
    QMatrix4x4 rotMat1;
    rotMat1.rotate(m_RotationA*4,rotAxis);
    QMatrix4x4 rotMat2;
    rotMat2.rotate(m_RotationB*4, rotAxis);

    mp_program->setUniformValue("uAlpha", m_alpha);
    mp_program->setUniformValue("uOffset",m_Udiff);
    mp_program->setUniformValue("uRotMat",rotMat1);

    // write unit indices to uniforms
    mp_program->setUniformValue(7, 0);

    // starting at vertex 0, render 3 vertices (=> 1 triangle)
    //glDrawArrays(GL_TRIANGLES, 0, 3);



    // draw first triangle
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    // draw second triangle
    mp_program2->bind();
    mp_program2->setUniformValue("uRotMat", rotMat2);
    mp_program2->setUniformValue("uAlpha", m_alpha);
    void* const offset = reinterpret_cast<void* const>(sizeof(GLuint)*3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, offset);
    mp_program2->release();

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
    updateProjectionMatrix();
}
void MyGLWidget::setProjectionMode(int perOrth) {
    if(perOrth == 1){
        this->m_ProjectionMode = 1;
        std::cout << "Perspective" << std::endl;
    }
    if(perOrth == 2) {
        this->m_ProjectionMode = 2;
        std::cout << "Orthogonal" << std::endl;
    }
    emit projectionModeChanged(perOrth);
}

void MyGLWidget::updateProjectionMatrix() {
    projectionMat = QMatrix4x4();
    projectionMat.perspective(qDegreesToRadians((double)m_Angle), m_Aspect, (float) m_Near, (float) m_Far);
}

void MyGLWidget::setNear(double value) {
    this->m_Near = value;

    if(value > m_Far-2)
        emit nearFar(0);

    emit nearValueChanged(value);
    updateProjectionMatrix();
}

void MyGLWidget::setFar(double value) {
    this->m_Far = value;

    if(value < m_Near+2)
        emit nearFar(1);

    emit farValueChanged(value);
    updateProjectionMatrix();
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
    this->m_Udiff = (float) (m_RotationC/100.0);
}

void MyGLWidget::finalizeGL() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteTextures(1, &m_tex);
    delete mp_program;
}
