#ifndef AWARDPAGE_H
#define AWARDPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>

class Awardpage : public QWidget
{
    Q_OBJECT

public:
    explicit Awardpage(QWidget *parent = nullptr);
    void loadAwards();

private slots:
    void addAward();
    void deleteAward();

private:
    QLineEdit *m_editTitle;
    QComboBox *m_comboLevel;
    QLineEdit *m_editDate;
    QLineEdit *m_editDesc;
    QPushButton *m_btnAdd;

    QTableWidget *m_tableAward;
    QPushButton *m_btnDelete;
};

#endif // AWARDPAGE_H
