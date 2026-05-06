#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "coursepage.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    CoursePage *m_coursePage;
};

#endif // MAINWINDOW_H
