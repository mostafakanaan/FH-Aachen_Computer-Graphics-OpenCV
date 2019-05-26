#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //FOV Slider
    connect(ui->vsFOV, &QSlider::valueChanged, ui->spFOV, QOverload<int>::of(&QSpinBox::setValue));
    connect(ui->vsFOV, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setFOV);
    //FOV Spinbox
    connect(ui->spFOV, QOverload<int>::of(&QSpinBox::valueChanged), ui->vsFOV, &QSlider::setValue);
    connect(ui->spFOV, QOverload<int>::of(&QSpinBox::valueChanged), ui->openGLWidget, &MyGLWidget::setFOV);
    //FOV Konsolenausgabe
    connect(ui->openGLWidget, &MyGLWidget::fovValueChanged,
            [](int value) {std::cout << value << std::endl;});

    //Angle Slider
    connect(ui->vsAngle, &QSlider::valueChanged, ui->spAngle, QOverload<int>::of(&QSpinBox::setValue));
    connect(ui->vsAngle, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setAngle);
    connect(ui->vsAngle, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setAlpha);

    //Angle Spinbox
    connect(ui->spAngle, QOverload<int>::of(&QSpinBox::valueChanged), ui->vsAngle, &QSlider::setValue);
    connect(ui->spAngle, QOverload<int>::of(&QSpinBox::valueChanged), ui->openGLWidget, &MyGLWidget::setAngle);
    //Angle Konsolenausgabe
    connect(ui->openGLWidget, &MyGLWidget::angleValueChanged,
           [](int value) {std::cout << value << std::endl;});

    //Rotation
    connect(ui->hsRotationA, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationA);
    connect(ui->hsRotationB, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationB);
    connect(ui->hsRotationC, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setRotationC);
    connect(ui->hsRotationC, &QSlider::valueChanged, ui->openGLWidget, &MyGLWidget::setUco);

    //Near/Far
    connect(ui->dsbNear, QOverload<double>::of(&QDoubleSpinBox::valueChanged), ui->openGLWidget, &MyGLWidget::setNear);
    connect(ui->dsbFar, QOverload<double>::of(&QDoubleSpinBox::valueChanged), ui->openGLWidget, &MyGLWidget::setFar);
    connect(ui->openGLWidget, &MyGLWidget::nearFar, [=](int nearfar) {ui->openGLWidget->fixNearFar(nearfar);});
    connect(ui->openGLWidget, &MyGLWidget::nearValueChanged, ui->dsbNear, &QDoubleSpinBox::setValue);
    connect(ui->openGLWidget, &MyGLWidget::farValueChanged, ui->dsbFar, &QDoubleSpinBox::setValue);

    //Reset
    connect(ui->rbReset, &QPushButton::clicked, this, &::MainWindow::reset);

    //Projection Mode
    connect(ui->rbPerspective, &QRadioButton::clicked, [=] () {ui->openGLWidget->setProjectionMode(1);});
    connect(ui->rbOrthogonal, &QRadioButton::clicked, [=] () {ui->openGLWidget->setProjectionMode(2);});
   }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset() {
    this->ui->dsbFar->setValue(0.00);
    this->ui->dsbNear->setValue(0.00);
    this->ui->hsRotationA->setValue(0);
    this->ui->hsRotationB->setValue(0);
    this->ui->hsRotationC->setValue(0);
    this->ui->vsFOV->setValue(45);
    this->ui->vsAngle->setValue(0);
    this->ui->rbPerspective->click();
}
