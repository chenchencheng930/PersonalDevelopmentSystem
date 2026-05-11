#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class QPushButton;
class QLabel;
class CoursePage;
class Experiencepage;
class Awardpage;
class AdvicePage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void switchPage(int index);
    void refreshDashboard();

private:
    void setupUi();
    QWidget* createSidebar();
    QWidget* createDashboard();
    void updateButtonStyles(int activeIndex);

    QStackedWidget *m_stack;

    QPushButton *m_btnDashboard;
    QPushButton *m_btnCourse;
    QPushButton *m_btnExperience;
    QPushButton *m_btnAward;
    QPushButton *m_btnAdvice;

    QList<QPushButton*> m_navButtons;

    QLabel *m_lblCourseCount;
    QLabel *m_lblGpa;
    QLabel *m_lblExpCount;
    QLabel *m_lblAwardCount;
    QLabel *m_lblAdviceSummary;

    QWidget *m_dashboardPage;
    CoursePage *m_coursePage;
    Experiencepage *m_experiencePage;
    Awardpage *m_awardPage;
    AdvicePage *m_advicePage;
};

#endif // MAINWINDOW_H
