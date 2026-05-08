#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"coursepage.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("大学生个人发展规划系统");
    this->resize(1000, 700);
    this->setMinimumSize(900, 600);

    CoursePage *coursePage = new CoursePage(this);
    this->setCentralWidget(coursePage);
}




MainWindow::~MainWindow()
{
    delete ui;
}
