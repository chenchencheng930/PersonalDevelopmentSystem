#include "coursepage.h"

CoursePage::CoursePage(QWidget *parent)
    : QWidget(parent)
{
    // -------------------------- 顶部输入区 --------------------------
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->setSpacing(12);
    inputLayout->setContentsMargins(10, 10, 10, 10);

    inputLayout->addWidget(new QLabel("课程名"));
    m_editName = new QLineEdit;
    m_editName->setPlaceholderText("请输入课程名");
    inputLayout->addWidget(m_editName, 1); // 拉伸系数1，自动占空间

    inputLayout->addWidget(new QLabel("学分"));
    m_editCredit = new QLineEdit;
    m_editCredit->setPlaceholderText("请输入学分");
    inputLayout->addWidget(m_editCredit, 1);

    inputLayout->addWidget(new QLabel("成绩"));
    m_editScore = new QLineEdit;
    m_editScore->setPlaceholderText("请输入成绩");
    inputLayout->addWidget(m_editScore, 1);

    inputLayout->addWidget(new QLabel("学期"));
    m_comboSemester = new QComboBox;
    m_comboSemester->addItems({
        "大一上", "大一下",
        "大二上", "大二下",
        "大三上", "大三下",
        "大四上", "大四下"
    });
    inputLayout->addWidget(m_comboSemester, 1);

    m_btnAdd = new QPushButton("添加");
    m_btnAdd->setFixedWidth(80);
    inputLayout->addWidget(m_btnAdd);

    // -------------------------- 中间表格区 --------------------------
    m_tableCourse = new QTableWidget;
    m_tableCourse->setColumnCount(4);
    m_tableCourse->setHorizontalHeaderLabels({"课程名", "学分", "成绩", "学期"});
    // 关键：列宽自动均分，窗口缩放不变形
    m_tableCourse->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableCourse->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableCourse->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // -------------------------- 底部操作+GPA区 --------------------------
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->setSpacing(20);
    bottomLayout->setContentsMargins(10, 10, 10, 10);

    m_btnDelete = new QPushButton("删除");
    m_btnDelete->setFixedWidth(100);
    m_btnRefresh = new QPushButton("刷新GPA");
    m_btnRefresh->setFixedWidth(100);

    bottomLayout->addWidget(m_btnDelete);
    bottomLayout->addWidget(m_btnRefresh);
    bottomLayout->addStretch(1); // 把GPA标签推到右侧

    m_labelTotalGPA = new QLabel("总GPA：0.00");
    m_labelCurrentGPA = new QLabel("当前学期GPA：0.00");
    bottomLayout->addWidget(m_labelTotalGPA);
    bottomLayout->addWidget(m_labelCurrentGPA);

    // -------------------------- 主布局（核心，防止控件乱跑） --------------------------
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(m_tableCourse, 1); // 表格占满剩余空间
    mainLayout->addLayout(bottomLayout);

    setWindowTitle("课程管理与GPA计算");
    resize(850, 600);

    // 绑定按钮事件
    connect(m_btnAdd, &QPushButton::clicked, this, &CoursePage::addCourse);
    connect(m_btnDelete, &QPushButton::clicked, this, &CoursePage::deleteCourse);
    connect(m_btnRefresh, &QPushButton::clicked, this, &CoursePage::refreshGPA);
}

// 添加课程
void CoursePage::addCourse()
{
    QString name = m_editName->text().trimmed();
    QString creditStr = m_editCredit->text().trimmed();
    QString scoreStr = m_editScore->text().trimmed();
    QString semester = m_comboSemester->currentText();

    if (name.isEmpty() || creditStr.isEmpty() || scoreStr.isEmpty()) return;

    bool ok1, ok2;
    double credit = creditStr.toDouble(&ok1);
    double score = scoreStr.toDouble(&ok2);
    if (!ok1 || !ok2 || credit <= 0 || score < 0 || score > 100) return;

    int row = m_tableCourse->rowCount();
    m_tableCourse->insertRow(row);
    m_tableCourse->setItem(row, 0, new QTableWidgetItem(name));
    m_tableCourse->setItem(row, 1, new QTableWidgetItem(creditStr));
    m_tableCourse->setItem(row, 2, new QTableWidgetItem(scoreStr));
    m_tableCourse->setItem(row, 3, new QTableWidgetItem(semester));

    m_editName->clear();
    m_editCredit->clear();
    m_editScore->clear();

    refreshGPA();
}

// 删除课程
void CoursePage::deleteCourse()
{
    int row = m_tableCourse->currentRow();
    if (row >= 0) {
        m_tableCourse->removeRow(row);
        refreshGPA();
    }
}

// 刷新GPA
void CoursePage::refreshGPA()
{
    double totalGPA = calculateGPA();
    m_labelTotalGPA->setText(QString("总GPA：%1").arg(totalGPA, 0, 'f', 2));

    QString currentSemester = m_comboSemester->currentText();
    double currentGPA = calculateGPA(currentSemester);
    m_labelCurrentGPA->setText(QString("当前学期GPA：%1").arg(currentGPA, 0, 'f', 2));
}
 // GPA计算工具函数
 double CoursePage::calculateGPA(const QString& semester) const
 {
     double totalCredit = 0;
     double totalPoint = 0;
     for (int i = 0; i < m_tableCourse->rowCount(); ++i) {
         if (!semester.isEmpty() && m_tableCourse->item(i, 3)->text() != semester)
             continue;
         double credit = m_tableCourse->item(i, 1)->text().toDouble();
         double score = m_tableCourse->item(i, 2)->text().toDouble();
         double gpa;
         if (score >= 90) gpa = 4.0;
         else if (score >= 85) gpa = 3.7;
         else if (score >= 82) gpa = 3.3;
         else if (score >= 78) gpa = 3.0;
         else if (score >= 75) gpa = 2.7;
         else if (score >= 72) gpa = 2.3;
         else if (score >= 68) gpa = 2.0;
         else if (score >= 64) gpa = 1.5;
         else if (score >= 60) gpa = 1.0;
         else gpa = 0.0;
         totalCredit += credit;
         totalPoint += credit * gpa;
     }
     return totalCredit > 0 ? totalPoint / totalCredit : 0.0;
 }
