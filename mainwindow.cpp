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
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

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
    refreshDashboard();
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

    // Export buttons
    QHBoxLayout *btnRow = new QHBoxLayout;
    btnRow->setSpacing(10);

    QPushButton *btnExport = new QPushButton("导出数据 (JSON)");
    btnExport->setStyleSheet(
        "QPushButton { background: #3498db; color: white; border: none;"
        "border-radius: 6px; padding: 10px 20px; font-size: 14px; }"
        "QPushButton:hover { background: #2980b9; }");
    btnExport->setCursor(Qt::PointingHandCursor);
    connect(btnExport, &QPushButton::clicked, this, &MainWindow::exportData);

    QPushButton *btnResume = new QPushButton("导出个人简历 (HTML)");
    btnResume->setStyleSheet(
        "QPushButton { background: #27ae60; color: white; border: none;"
        "border-radius: 6px; padding: 10px 20px; font-size: 14px; }"
        "QPushButton:hover { background: #219a52; }");
    btnResume->setCursor(Qt::PointingHandCursor);
    connect(btnResume, &QPushButton::clicked, this, &MainWindow::exportResume);

    btnRow->addWidget(btnExport);
    btnRow->addWidget(btnResume);
    btnRow->addStretch();
    layout->addLayout(btnRow);

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
    } else if (index == 1) {
        // refresh course page data
    } else if (index == 2) {
        m_experiencePage->loadExperiences();
    } else if (index == 3) {
        m_awardPage->loadAwards();
    } else if (index == 4) {
        refreshDashboard();
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

void MainWindow::exportData()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "导出数据", "personal_development_data.json",
        "JSON Files (*.json)");

    if (fileName.isEmpty()) return;

    QJsonObject root;

    // 课程数据
    QJsonArray courses;
    QSqlQuery courseQuery = GpaCalculator::instance().getAllCourses();
    while (courseQuery.next()) {
        QJsonObject c;
        c["id"] = courseQuery.value("id").toInt();
        c["name"] = courseQuery.value("name").toString();
        c["credit"] = courseQuery.value("credit").toDouble();
        c["score"] = courseQuery.value("score").toDouble();
        c["semester"] = courseQuery.value("semester").toString();
        courses.append(c);
    }
    root["courses"] = courses;

    // 经历数据
    QJsonArray experiences;
    QSqlQuery expQuery = GpaCalculator::instance().getAllExperiences();
    while (expQuery.next()) {
        QJsonObject e;
        e["id"] = expQuery.value("id").toInt();
        e["type"] = expQuery.value("type").toString();
        e["title"] = expQuery.value("title").toString();
        e["date"] = expQuery.value("date").toString();
        e["role"] = expQuery.value("role").toString();
        e["description"] = expQuery.value("description").toString();
        experiences.append(e);
    }
    root["experiences"] = experiences;

    // 奖项数据
    QJsonArray awards;
    QSqlQuery awardQuery = GpaCalculator::instance().getAllAwards();
    while (awardQuery.next()) {
        QJsonObject a;
        a["id"] = awardQuery.value("id").toInt();
        a["title"] = awardQuery.value("title").toString();
        a["level"] = awardQuery.value("level").toString();
        a["date"] = awardQuery.value("date").toString();
        a["description"] = awardQuery.value("description").toString();
        awards.append(a);
    }
    root["awards"] = awards;

    // GPA 摘要
    QJsonObject summary;
    summary["totalGPA"] = GpaCalculator::instance().getTotalGpa();
    summary["courseCount"] = GpaCalculator::instance().getCourseCount();
    summary["experienceCount"] = GpaCalculator::instance().getExperienceCount();
    summary["awardCount"] = GpaCalculator::instance().getAwardCount();
    summary["hasInternship"] = GpaCalculator::instance().hasInternship();
    root["summary"] = summary;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法创建文件");
        return;
    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    file.close();

    QMessageBox::information(this, "导出成功",
                             QString("数据已导出到：\n%1").arg(fileName));
}

void MainWindow::exportResume()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, "导出个人简历", "my_resume.html",
        "HTML Files (*.html)");

    if (fileName.isEmpty()) return;

    GpaCalculator &gpa = GpaCalculator::instance();
    double totalGpa = gpa.getTotalGpa();
    int courseCount = gpa.getCourseCount();
    int expCount = gpa.getExperienceCount();
    int awardCount = gpa.getAwardCount();
    bool hasIntern = gpa.hasInternship();

    // 构建课程表格行
    QString courseRows;
    QSqlQuery courseQuery = gpa.getAllCourses();
    while (courseQuery.next()) {
        courseRows += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td></tr>")
            .arg(courseQuery.value("name").toString())
            .arg(courseQuery.value("credit").toDouble())
            .arg(courseQuery.value("score").toDouble())
            .arg(courseQuery.value("semester").toString());
    }

    // 构建经历列表
    QString expItems;
    QSqlQuery expQuery = gpa.getAllExperiences();
    while (expQuery.next()) {
        expItems += QString("<li><b>[%1] %2</b> (%3) - %4<br/>%5</li>")
            .arg(expQuery.value("type").toString())
            .arg(expQuery.value("title").toString())
            .arg(expQuery.value("date").toString())
            .arg(expQuery.value("role").toString())
            .arg(expQuery.value("description").toString());
    }

    // 构建奖项列表
    QString awardItems;
    QSqlQuery awardQuery = gpa.getAllAwards();
    while (awardQuery.next()) {
        awardItems += QString("<li><b>%1</b> [%2] (%3) - %4</li>")
            .arg(awardQuery.value("title").toString())
            .arg(awardQuery.value("level").toString())
            .arg(awardQuery.value("date").toString())
            .arg(awardQuery.value("description").toString());
    }

    QString html = QString(R"(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>个人简历</title>
    <style>
        body { font-family: 'Microsoft YaHei', sans-serif; max-width: 800px; margin: 0 auto; padding: 30px; color: #333; }
        h1 { text-align: center; color: #2c3e50; border-bottom: 3px solid #3498db; padding-bottom: 10px; }
        h2 { color: #2980b9; border-bottom: 1px solid #bdc3c7; padding-bottom: 5px; margin-top: 25px; }
        .summary { display: flex; justify-content: space-around; background: #ecf0f1; border-radius: 10px; padding: 15px; margin: 15px 0; }
        .summary-item { text-align: center; }
        .summary-item .val { font-size: 22px; font-weight: bold; color: #2c3e50; }
        .summary-item .lbl { font-size: 12px; color: #7f8c8d; }
        table { width: 100%%; border-collapse: collapse; margin: 10px 0; }
        th, td { border: 1px solid #ddd; padding: 8px 12px; text-align: left; }
        th { background: #3498db; color: white; }
        tr:nth-child(even) { background: #f2f2f2; }
        ul { line-height: 1.8; }
        .footer { text-align: center; color: #95a5a6; font-size: 12px; margin-top: 30px; }
    </style>
</head>
<body>
    <h1>个人简历</h1>

    <div class="summary">
        <div class="summary-item"><div class="val">%1</div><div class="lbl">课程数量</div></div>
        <div class="summary-item"><div class="val">%2</div><div class="lbl">总 GPA</div></div>
        <div class="summary-item"><div class="val">%3</div><div class="lbl">实践经历</div></div>
        <div class="summary-item"><div class="val">%4</div><div class="lbl">获奖荣誉</div></div>
        <div class="summary-item"><div class="val">%5</div><div class="lbl">实习经历</div></div>
    </div>

    <h2>教育背景</h2>
    <p>本科在读 | GPA：%2</p>

    <h2>课程成绩</h2>
    <table>
        <tr><th>课程名称</th><th>学分</th><th>成绩</th><th>学期</th></tr>
        %6
    </table>

    <h2>实践经历</h2>
    <ul>%7</ul>

    <h2>获奖荣誉</h2>
    <ul>%8</ul>

    <div class="footer">
        <p>本简历由「大学生个人发展规划系统」自动生成</p>
    </div>
</body>
</html>
    )").arg(courseCount)
       .arg(totalGpa, 0, 'f', 2)
       .arg(expCount)
       .arg(awardCount)
       .arg(hasIntern ? "有" : "无")
       .arg(courseRows.isEmpty() ? "<tr><td colspan='4'>暂无课程记录</td></tr>" : courseRows)
       .arg(expItems.isEmpty() ? "<li>暂无实践经历</li>" : expItems)
       .arg(awardItems.isEmpty() ? "<li>暂无获奖记录</li>" : awardItems);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法创建文件");
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << html;
    file.close();

    QMessageBox::information(this, "导出成功",
                             QString("简历已导出到：\n%1").arg(fileName));
}
