#include "coursepage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <QAbstractItemView>

CoursePage::CoursePage(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(900, 600);

    QLabel *titleLabel = new QLabel("课程与成绩管理");
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold;");

    QLabel *nameLabel = new QLabel("课程名：");
    editCourseName = new QLineEdit;
    editCourseName->setPlaceholderText("请输入课程名");

    QLabel *creditLabel = new QLabel("学分：");
    editCredit = new QLineEdit;
    editCredit->setPlaceholderText("如 3");

    QLabel *scoreLabel = new QLabel("成绩：");
    editScore = new QLineEdit;
    editScore->setPlaceholderText("0-100");

    QLabel *semesterLabel = new QLabel("学期：");
    comboSemester = new QComboBox;
    comboSemester->addItems(QStringList()
                            << "大一上" << "大一下"
                            << "大二上" << "大二下"
                            << "大三上" << "大三下"
                            << "大四上" << "大四下");

    btnAddCourse = new QPushButton("添加课程");

    QGridLayout *inputLayout = new QGridLayout;
    inputLayout->addWidget(nameLabel, 0, 0);
    inputLayout->addWidget(editCourseName, 0, 1);
    inputLayout->addWidget(creditLabel, 0, 2);
    inputLayout->addWidget(editCredit, 0, 3);
    inputLayout->addWidget(scoreLabel, 0, 4);
    inputLayout->addWidget(editScore, 0, 5);
    inputLayout->addWidget(semesterLabel, 0, 6);
    inputLayout->addWidget(comboSemester, 0, 7);
    inputLayout->addWidget(btnAddCourse, 0, 8);

    inputLayout->setColumnStretch(1, 2);
    inputLayout->setColumnStretch(3, 1);
    inputLayout->setColumnStretch(5, 1);
    inputLayout->setColumnStretch(7, 1);

    tableCourse = new QTableWidget;
    tableCourse->setColumnCount(4);
    tableCourse->setHorizontalHeaderLabels(QStringList()
                                           << "课程名"
                                           << "学分"
                                           << "成绩"
                                           << "学期");
    tableCourse->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableCourse->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableCourse->setSelectionMode(QAbstractItemView::SingleSelection);
    tableCourse->setEditTriggers(QAbstractItemView::NoEditTriggers);

    btnDeleteCourse = new QPushButton("删除课程");
    btnRefreshCourse = new QPushButton("刷新");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(btnDeleteCourse);
    buttonLayout->addWidget(btnRefreshCourse);
    buttonLayout->addStretch();

    lblTotalGpa = new QLabel("总 GPA：0.00");
    lblSemesterGpa = new QLabel("当前学期 GPA：0.00");

    QHBoxLayout *gpaLayout = new QHBoxLayout;
    gpaLayout->addWidget(lblTotalGpa);
    gpaLayout->addSpacing(30);
    gpaLayout->addWidget(lblSemesterGpa);
    gpaLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(tableCourse);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(gpaLayout);

    setLayout(mainLayout);

    connect(btnAddCourse, &QPushButton::clicked,
            this, &CoursePage::onAddCourseClicked);

    connect(btnDeleteCourse, &QPushButton::clicked,
            this, &CoursePage::onDeleteCourseClicked);

    connect(btnRefreshCourse, &QPushButton::clicked,
            this, &CoursePage::onRefreshCourseClicked);

    updateGpaDisplay();
}

void CoursePage::onAddCourseClicked()
{
    QString name = editCourseName->text().trimmed();

    bool creditOk = false;
    bool scoreOk = false;

    double credit = editCredit->text().toDouble(&creditOk);
    double score = editScore->text().toDouble(&scoreOk);
    QString semester = comboSemester->currentText();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "课程名不能为空！");
        return;
    }

    if (!creditOk || credit <= 0) {
        QMessageBox::warning(this, "输入错误", "学分必须是大于 0 的数字！");
        return;
    }

    if (!scoreOk || score < 0 || score > 100) {
        QMessageBox::warning(this, "输入错误", "成绩必须在 0 到 100 之间！");
        return;
    }

    int row = tableCourse->rowCount();
    tableCourse->insertRow(row);

    tableCourse->setItem(row, 0, new QTableWidgetItem(name));
    tableCourse->setItem(row, 1, new QTableWidgetItem(QString::number(credit, 'f', 1)));
    tableCourse->setItem(row, 2, new QTableWidgetItem(QString::number(score, 'f', 1)));
    tableCourse->setItem(row, 3, new QTableWidgetItem(semester));

    editCourseName->clear();
    editCredit->clear();
    editScore->clear();

    updateGpaDisplay();
}

void CoursePage::onDeleteCourseClicked()
{
    int row = tableCourse->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "删除失败", "请先选择要删除的课程！");
        return;
    }

    int ret = QMessageBox::question(this,
                                    "确认删除",
                                    "确定要删除选中的课程吗？",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        tableCourse->removeRow(row);
        updateGpaDisplay();
    }
}

void CoursePage::onRefreshCourseClicked()
{
    updateGpaDisplay();
    QMessageBox::information(this, "刷新成功", "课程列表已刷新。");
}

double CoursePage::scoreToGpa(double score) const
{
    if (score >= 90) return 4.0;
    if (score >= 85) return 3.7;
    if (score >= 82) return 3.3;
    if (score >= 78) return 3.0;
    if (score >= 75) return 2.7;
    if (score >= 72) return 2.3;
    if (score >= 68) return 2.0;
    if (score >= 64) return 1.5;
    if (score >= 60) return 1.0;
    return 0.0;
}

void CoursePage::updateGpaDisplay()
{
    double totalCredit = 0.0;
    double totalPoint = 0.0;

    double semesterCredit = 0.0;
    double semesterPoint = 0.0;

    QString currentSemester = comboSemester->currentText();

    for (int row = 0; row < tableCourse->rowCount(); ++row) {
        QTableWidgetItem *creditItem = tableCourse->item(row, 1);
        QTableWidgetItem *scoreItem = tableCourse->item(row, 2);
        QTableWidgetItem *semesterItem = tableCourse->item(row, 3);

        if (!creditItem || !scoreItem || !semesterItem) {
            continue;
        }

        double credit = creditItem->text().toDouble();
        double score = scoreItem->text().toDouble();
        double gpa = scoreToGpa(score);

        totalCredit += credit;
        totalPoint += credit * gpa;

        if (semesterItem->text() == currentSemester) {
            semesterCredit += credit;
            semesterPoint += credit * gpa;
        }
    }

    double totalGpa = 0.0;
    if (totalCredit > 0) {
        totalGpa = totalPoint / totalCredit;
    }

    double semesterGpa = 0.0;
    if (semesterCredit > 0) {
        semesterGpa = semesterPoint / semesterCredit;
    }

    lblTotalGpa->setText(QString("总 GPA：%1").arg(totalGpa, 0, 'f', 2));
    lblSemesterGpa->setText(QString("当前学期 GPA：%1").arg(semesterGpa, 0, 'f', 2));
}
