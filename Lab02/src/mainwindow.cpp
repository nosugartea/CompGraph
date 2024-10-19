#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Lab02");
    setFixedSize(940, 600);
    myView = new MyGraphicView();
    ui->graphicLayout->addWidget(myView);

    connect(ui->drawButton, &QPushButton::clicked, this, &MainWindow::onDrawButtonClick);
    connect(ui->cleanButton, &QPushButton::clicked, this, &MainWindow::onCleanButtonClick);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onDrawButtonClick()
{
    myView->makeBezierCurve();
}

void MainWindow::onCleanButtonClick()
{
    myView->cleanAll();
}
