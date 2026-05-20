#include "experiencepage.h"
#include "ui_experiencepage.h"

Experiencepage::Experiencepage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Experiencepage)
{
    ui->setupUi(this);
}

Experiencepage::~Experiencepage()
{
    delete ui;
}
// 3号任务：经历和奖项模块
