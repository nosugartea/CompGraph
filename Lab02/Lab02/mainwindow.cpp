#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(940, 600);
    myPicture   = new MyGraphicView();
    ui->graphicLayout->addWidget(myPicture);

    connect(ui->drawButton, &QPushButton::clicked, this, &MainWindow::onDrawButtonClick);
    connect(ui->cleanButton, &QPushButton::clicked, this, &MainWindow::onCleanButtonClick);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onDrawButtonClick()
{
    myPicture->makeBezierCurve();
}

void MainWindow::onCleanButtonClick()
{
    myPicture->cleanAll();
}
