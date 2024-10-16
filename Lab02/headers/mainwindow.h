#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <headers/mygraphicview.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDrawButtonClick();
    void onCleanButtonClick();

private:
    Ui::MainWindow *ui;
    MyGraphicView  *myView;
};
#endif // MAINWINDOW_H
