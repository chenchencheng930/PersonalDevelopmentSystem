#ifndef AWARDPAGE_H
#define AWARDPAGE_H

#include <QWidget>

namespace Ui {
class Awardpage;
}

class Awardpage : public QWidget
{
    Q_OBJECT

public:
    explicit Awardpage(QWidget *parent = 0);
    ~Awardpage();

private:
    Ui::Awardpage *ui;
};

#endif // AWARDPAGE_H
