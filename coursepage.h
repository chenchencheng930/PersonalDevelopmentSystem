#ifndef COURSEPAGE_H
#define COURSEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>

class CoursePage : public QWidget
{
    Q_OBJECT
public:
    explicit CoursePage(QWidget *parent = nullptr);

private slots:
    void addCourse();
    void deleteCourse();
    void refreshGPA();

private:
    // 输入区
    QLineEdit *m_editName;
    QLineEdit *m_editCredit;
    QLineEdit *m_editScore;
    QComboBox *m_comboSemester;
    QPushButton *m_btnAdd;

    // 表格区
    QTableWidget *m_tableCourse;
    QPushButton *m_btnDelete;
    QPushButton *m_btnRefresh;

    // GPA显示区
    QLabel *m_labelTotalGPA;
    QLabel *m_labelCurrentGPA;

    // GPA计算函数
    double calculateGPA(const QString& semester = "") const;
};

#endif // COURSEPAGE_H
