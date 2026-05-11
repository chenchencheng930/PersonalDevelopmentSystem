#ifndef ADVICEPAGE_H
#define ADVICEPAGE_H

#include <QWidget>

class QLabel;

class AdvicePage : public QWidget
{
    Q_OBJECT

public:
    explicit AdvicePage(QWidget *parent = nullptr);
    void refresh();

private:
    QString generateAcademicAdvice() const;
    QString generatePracticeAdvice() const;
    QString generateHonorAdvice() const;
    QString generateSummaryAdvice() const;

    QLabel *m_titleLabel;
    QLabel *m_academicLabel;
    QLabel *m_practiceLabel;
    QLabel *m_honorLabel;
    QLabel *m_summaryLabel;
};

#endif // ADVICEPAGE_H
