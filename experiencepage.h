#ifndef EXPERIENCEPAGE_H
#define EXPERIENCEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>

class Experiencepage : public QWidget
{
    Q_OBJECT

public:
    explicit Experiencepage(QWidget *parent = nullptr);
    void loadExperiences();

private slots:
    void addExperience();
    void deleteExperience();

private:
    QComboBox *m_comboType;
    QLineEdit *m_editTitle;
    QLineEdit *m_editDate;
    QLineEdit *m_editRole;
    QLineEdit *m_editDesc;
    QPushButton *m_btnAdd;

    QTableWidget *m_tableExperience;
    QPushButton *m_btnDelete;
};

#endif // EXPERIENCEPAGE_H
