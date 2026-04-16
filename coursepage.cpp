#include "coursepage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>

CoursePage::CoursePage(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("课程管理");
    resize(800, 500);

    // 1. 输入区
    QLineEdit *editName = new QLineEdit(this);
    QLineEdit *editCredit = new QLineEdit(this);
    QLineEdit *editScore = new QLineEdit(this);
    QComboBox *comboTerm = new QComboBox(this);
    QPushButton *btnAdd = new QPushButton("添加", this);

    comboTerm->addItems({"大一上","大一下","大二上","大二下","大三上","大三下","大四上","大四下"});

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(new QLabel("课程名:"));
    inputLayout->addWidget(editName);
    inputLayout->addWidget(new QLabel("学分:"));
    inputLayout->addWidget(editCredit);
    inputLayout->addWidget(new QLabel("成绩:"));
    inputLayout->addWidget(editScore);
    inputLayout->addWidget(new QLabel("学期:"));
    inputLayout->addWidget(comboTerm);
    inputLayout->addWidget(btnAdd);
    inputLayout->setSpacing(10);

    // 2. 表格区
    QTableWidget *table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"课程名","学分","成绩","学期"});
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QPushButton *btnDel = new QPushButton("删除", this);
    QPushButton *btnRefresh = new QPushButton("刷新", this);
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnDel);
    btnLayout->addWidget(btnRefresh);
    btnLayout->addStretch();
    btnLayout->setSpacing(10);

    // 3. GPA区
    QLabel *labTotal = new QLabel("总GPA：0.00", this);
    QLabel *labTerm = new QLabel("当前学期GPA：0.00", this);
    QHBoxLayout *gpaLayout = new QHBoxLayout;
    gpaLayout->addWidget(labTotal);
    gpaLayout->addWidget(labTerm);
    gpaLayout->addStretch();
    gpaLayout->setSpacing(30);

    // 4. 总布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(table);
    mainLayout->addLayout(btnLayout);
    mainLayout->addLayout(gpaLayout);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(15,15,15,15);

    setLayout(mainLayout);
}
