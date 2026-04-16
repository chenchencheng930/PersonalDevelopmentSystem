#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"coursepage.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CoursePage *coursePage = new CoursePage(this);
        coursePage->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
