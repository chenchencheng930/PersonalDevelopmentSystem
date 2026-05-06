#include "mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    m_coursePage = new CoursePage(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_coursePage);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    setWindowTitle("个人发展系统 - 课程管理");
    resize(850, 600);
}
