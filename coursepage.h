#ifndef COURSEPAGE_H
#define COURSEPAGE_H

#include <QWidget>

class QLineEdit;
class QComboBox;
class QPushButton;
class QTableWidget;
class QLabel;

class CoursePage : public QWidget
{
    Q_OBJECT

public:
    explicit CoursePage(QWidget *parent = nullptr);

private slots:
    void onAddCourseClicked();
    void onDeleteCourseClicked();
    void onRefreshCourseClicked();

private:
    void updateGpaDisplay();
    double scoreToGpa(double score) const;

private:
    QLineEdit *editCourseName;
    QLineEdit *editCredit;
    QLineEdit *editScore;
    QComboBox *comboSemester;

    QPushButton *btnAddCourse;
    QPushButton *btnDeleteCourse;
    QPushButton *btnRefreshCourse;

    QTableWidget *tableCourse;

    QLabel *lblTotalGpa;
    QLabel *lblSemesterGpa;
};

#endif // COURSEPAGE_H
