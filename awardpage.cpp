#include "awardpage.h"
#include "ui_awardpage.h"

Awardpage::Awardpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Awardpage)
{
    ui->setupUi(this);
}

Awardpage::~Awardpage()
{
    delete ui;
}
