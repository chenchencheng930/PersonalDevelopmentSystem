#include "mainwindow.h"
#include "coursepage.h"
#include "experiencepage.h"
#include "awardpage.h"
#include "advicepage.h"
#include "gpacalculator.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("大学生个人发展规划系统");
    resize(1000, 700);
    setMinimumSize(900, 600);
    setupUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(createSidebar());

    m_stack = new QStackedWidget;
    m_stack->setStyleSheet("background: #f5f6fa;");

    m_dashboardPage = createDashboard();
    m_coursePage = new CoursePage;
    m_experiencePage = new Experiencepage;
    m_awardPage = new Awardpage;
    m_advicePage = new AdvicePage;

    m_stack->addWidget(m_dashboardPage);
    m_stack->addWidget(m_coursePage);
    m_stack->addWidget(m_experiencePage);
    m_stack->addWidget(m_awardPage);
    m_stack->addWidget(m_advicePage);

    mainLayout->addWidget(m_stack, 1);

    setCentralWidget(central);

    m_stack->setCurrentIndex(0);
    updateButtonStyles(0);
}

QWidget* MainWindow::createSidebar()
{
    QWidget *sidebar = new QWidget;
    sidebar->setFixedWidth(160);
    sidebar->setStyleSheet("background: #2c3e50;");

    QVBoxLayout *layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(8, 20, 8, 20);
    layout->setSpacing(6);

    QLabel *logo = new QLabel("发展导航");
    logo->setStyleSheet("color: white; font-size: 16px; font-weight: bold; padding: 10px;");
    logo->setAlignment(Qt::AlignCenter);
    layout->addWidget(logo);

    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #7f8c8d;");
    layout->addWidget(line);
    layout->addSpacing(6);

    QString btnStyle =
        "QPushButton {"
        "  color: #bdc3c7; background: transparent; border: none;"
        "  border-radius: 6px; padding: 10px 12px; text-align: left; font-size: 14px;"
        "}"
        "QPushButton:hover { background: #34495e; color: white; }"
        "QPushButton:checked { background: #3498db; color: white; font-weight: bold; }";

    m_btnDashboard   = new QPushButton("  首页概览");;
    m_btnCourse      = new QPushButton("  课程成绩");
    m_btnExperience  = new QPushButton("  经历管理");
    m_btnAward       = new QPushButton("  奖项荣誉");
    m_btnAdvice      = new QPushButton("  成长建议");

    m_navButtons = {m_btnDashboard, m_btnCourse, m_btnExperience, m_btnAward, m_btnAdvice};

    for (int i = 0; i < m_navButtons.size(); ++i) {
        QPushButton *btn = m_navButtons[i];
        btn->setCheckable(true);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMinimumHeight(40);
        btn->setStyleSheet(btnStyle);
        layout->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, [this, i]() {
            switchPage(i);
        });
    }

    layout->addStretch();
    return sidebar;
}

QWidget* MainWindow::createDashboard()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 20, 30, 20);
    layout->setSpacing(20);

    QLabel *title = new QLabel("个人发展规划概览");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #2c3e50;");
    layout->addWidget(title);

    // Stats cards
    QHBoxLayout *statsRow = new QHBoxLayout;
    statsRow->setSpacing(15);

    struct Card {
        QFrame *frame;
        QLabel *valueLabel;
    };

    auto makeCard = [](const QString &title, const QString &color) -> Card {
        QFrame *frame = new QFrame;
        frame->setStyleSheet(QString(
            "QFrame { background: white; border-radius: 10px;"
            "border-left: 5px solid %1; padding: 15px; }").arg(color));
        frame->setMinimumSize(180, 100);

        QVBoxLayout *cl = new QVBoxLayout(frame);
        QLabel *tl = new QLabel(title);
        tl->setStyleSheet("font-size: 13px; color: #7f8c8d; border: none;");
        QLabel *vl = new QLabel("--");
        vl->setStyleSheet(QString("font-size: 28px; font-weight: bold; color: %1; border: none;").arg(color));
        cl->addWidget(tl);
        cl->addWidget(vl);
        cl->addStretch();
        return {frame, vl};
    };

    Card c1 = makeCard("课程数量", "#3498db");
    Card c2 = makeCard("总 GPA", "#27ae60");
    Card c3 = makeCard("经历数量", "#e67e22");
    Card c4 = makeCard("奖项数量", "#9b59b6");

    m_lblCourseCount = c1.valueLabel;
    m_lblGpa         = c2.valueLabel;
    m_lblExpCount    = c3.valueLabel;
    m_lblAwardCount  = c4.valueLabel;

    statsRow->addWidget(c1.frame);
    statsRow->addWidget(c2.frame);
    statsRow->addWidget(c3.frame);
    statsRow->addWidget(c4.frame);
    layout->addLayout(statsRow);

    // Advice summary
    QLabel *adviceTitle = new QLabel("成长建议摘要");
    adviceTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; margin-top: 10px;");
    layout->addWidget(adviceTitle);

    m_lblAdviceSummary = new QLabel;
    m_lblAdviceSummary->setWordWrap(true);
    m_lblAdviceSummary->setMinimumHeight(120);
    m_lblAdviceSummary->setStyleSheet(
        "font-size: 14px; color: #34495e; background: white;"
        "border-radius: 8px; padding: 15px; line-height: 1.6;");
    layout->addWidget(m_lblAdviceSummary);

    layout->addStretch();
    return page;
}

void MainWindow::switchPage(int index)
{
    m_stack->setCurrentIndex(index);
    updateButtonStyles(index);

    if (index == 0) {
        refreshDashboard();
    } else if (index == 4) {
        m_advicePage->refresh();
    }
}

void MainWindow::refreshDashboard()
{
    GpaCalculator &gpa = GpaCalculator::instance();

    int courseCount = gpa.getCourseCount();
    double totalGpa = gpa.getTotalGpa();
    int expCount = gpa.getExperienceCount();
    int awardCount = gpa.getAwardCount();

    m_lblCourseCount->setText(QString::number(courseCount));
    m_lblGpa->setText(QString::number(totalGpa, 'f', 2));
    m_lblExpCount->setText(QString::number(expCount));
    m_lblAwardCount->setText(QString::number(awardCount));

    QStringList tips;
    if (courseCount == 0) {
        tips << "尚未录入课程，请前往「课程成绩」页面添加课程记录。";
    }
    if (expCount == 0) {
        tips << "尚未录入经历，请前往「经历管理」页面添加实践经历。";
    }
    if (awardCount == 0) {
        tips << "尚未录入奖项，请前往「奖项荣誉」页面添加获奖记录。";
    }
    if (tips.isEmpty()) {
        tips << "各项数据已录入，点击「成长建议」查看详细的成长画像与发展建议。";
    }

    m_lblAdviceSummary->setText(tips.join("\n"));
}

void MainWindow::updateButtonStyles(int activeIndex)
{
    for (int i = 0; i < m_navButtons.size(); ++i) {
        m_navButtons[i]->setChecked(i == activeIndex);
    }
}
