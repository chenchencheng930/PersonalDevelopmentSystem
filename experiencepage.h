#ifndef EXPERIENCEPAGE_H
#define EXPERIENCEPAGE_H

#include <QWidget>

namespace Ui {
class Experiencepage;
}

class Experiencepage : public QWidget
{
    Q_OBJECT

public:
    explicit Experiencepage(QWidget *parent = 0);
    ~Experiencepage();

private:
    Ui::Experiencepage *ui;
};

#endif // EXPERIENCEPAGE_H
