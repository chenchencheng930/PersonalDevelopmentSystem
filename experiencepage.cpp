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
