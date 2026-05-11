#include "advicepage.h"
#include "gpacalculator.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

AdvicePage::AdvicePage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(30, 20, 30, 20);
    mainLayout->setSpacing(12);

    m_titleLabel = new QLabel("成长画像与发展建议");
    m_titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #2c3e50;");
    mainLayout->addWidget(m_titleLabel);

    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #bdc3c7;");
    mainLayout->addWidget(line);

    // Academic dimension
    QLabel *academicTitle = new QLabel("学业维度");
    academicTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #2980b9;");
    mainLayout->addWidget(academicTitle);

    m_academicLabel = new QLabel;
    m_academicLabel->setWordWrap(true);
    m_academicLabel->setStyleSheet(
        "font-size: 14px; color: #34495e; padding: 10px;"
        "background: #eaf2f8; border-radius: 6px; border-left: 4px solid #2980b9;");
    mainLayout->addWidget(m_academicLabel);

    // Practice dimension
    QLabel *practiceTitle = new QLabel("实践维度");
    practiceTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #27ae60;");
    mainLayout->addWidget(practiceTitle);

    m_practiceLabel = new QLabel;
    m_practiceLabel->setWordWrap(true);
    m_practiceLabel->setStyleSheet(
        "font-size: 14px; color: #34495e; padding: 10px;"
        "background: #eafaf1; border-radius: 6px; border-left: 4px solid #27ae60;");
    mainLayout->addWidget(m_practiceLabel);

    // Honor dimension
    QLabel *honorTitle = new QLabel("荣誉维度");
    honorTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #e67e22;");
    mainLayout->addWidget(honorTitle);

    m_honorLabel = new QLabel;
    m_honorLabel->setWordWrap(true);
    m_honorLabel->setStyleSheet(
        "font-size: 14px; color: #34495e; padding: 10px;"
        "background: #fef5e7; border-radius: 6px; border-left: 4px solid #e67e22;");
    mainLayout->addWidget(m_honorLabel);

    // Summary
    QLabel *summaryTitle = new QLabel("综合建议");
    summaryTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #8e44ad;");
    mainLayout->addWidget(summaryTitle);

    m_summaryLabel = new QLabel;
    m_summaryLabel->setWordWrap(true);
    m_summaryLabel->setStyleSheet(
        "font-size: 14px; color: #34495e; padding: 10px;"
        "background: #f4ecf7; border-radius: 6px; border-left: 4px solid #8e44ad;");
    mainLayout->addWidget(m_summaryLabel);

    mainLayout->addStretch();
    setLayout(mainLayout);

    refresh();
}

void AdvicePage::refresh()
{
    m_academicLabel->setText(generateAcademicAdvice());
    m_practiceLabel->setText(generatePracticeAdvice());
    m_honorLabel->setText(generateHonorAdvice());
    m_summaryLabel->setText(generateSummaryAdvice());
}

QString AdvicePage::generateAcademicAdvice() const
{
    GpaCalculator &gpa = GpaCalculator::instance();
    double totalGpa = gpa.getTotalGpa();
    int courseCount = gpa.getCourseCount();

    QStringList advice;

    if (courseCount == 0) {
        advice << "尚未录入课程数据，建议尽快补充完整的课程记录，以便系统进行学业分析。";
    } else {
        if (totalGpa >= 3.7) {
            advice << QString("当前总 GPA 为 %1，学业表现优秀，建议继续保持并冲刺更高目标。")
                          .arg(totalGpa, 0, 'f', 2);
        } else if (totalGpa >= 3.0) {
            advice << QString("当前总 GPA 为 %1，学业表现良好，仍有提升空间，建议重点关注低分课程，争取将 GPA 提升至 3.5 以上。")
                          .arg(totalGpa, 0, 'f', 2);
        } else if (totalGpa >= 2.0) {
            advice << QString("当前总 GPA 为 %1，学业表现一般，建议优先提升学业成绩，合理分配学习时间，减少低分课程的影响。")
                          .arg(totalGpa, 0, 'f', 2);
        } else if (courseCount > 0) {
            advice << QString("当前总 GPA 为 %1，学业成绩需要重点关注，建议制定详细的学习计划，针对薄弱科目加强学习。")
                          .arg(totalGpa, 0, 'f', 2);
        }

        if (courseCount < 5) {
            advice << "课程数量较少，建议补充完整课程记录，便于长期学业追踪与趋势分析。";
        }
    }

    return advice.isEmpty() ? "暂无学业数据。" : advice.join("\n\n");
}

QString AdvicePage::generatePracticeAdvice() const
{
    GpaCalculator &gpa = GpaCalculator::instance();
    int expCount = gpa.getExperienceCount();
    bool hasIntern = gpa.hasInternship();

    QStringList advice;

    if (expCount == 0) {
        advice << "尚未录入任何实践经历。建议积极参与项目开发、学科竞赛或企业实习，积累实践经验并为求职做好准备。";
    } else {
        if (!hasIntern) {
            advice << "暂无实习经历，建议关注企业实习机会，提前准备简历和面试，增加实际工作经验。";
        } else {
            advice << "已有实习经历，这是很好的职场起点，建议总结实习收获，提炼核心能力，在后续求职中充分展示。";
        }

        if (expCount < 2) {
            advice << "实践经历数量偏少，建议补充项目经历或参加学科竞赛，丰富个人履历的多样性。";
        } else if (expCount >= 3) {
            advice << "实践经历较为丰富，建议注重经历质量，提炼每项经历的核心收获和能力成长点，形成清晰的个人能力图谱。";
        }
    }

    return advice.join("\n\n");
}

QString AdvicePage::generateHonorAdvice() const
{
    GpaCalculator &gpa = GpaCalculator::instance();
    int awardCount = gpa.getAwardCount();

    QStringList advice;

    if (awardCount == 0) {
        advice << "尚未获得任何奖项荣誉。建议积极参加学科竞赛、申请奖学金或参与评优评先，提高个人竞争力。";
    } else if (awardCount == 1) {
        advice << "已获得 1 项奖项，这是一个好的开始，建议继续参加各类竞赛和评选活动，丰富荣誉记录。";
    } else if (awardCount < 4) {
        advice << QString("已获得 %1 项奖项，荣誉积累有一定基础，建议关注更高级别的竞赛和评选，提升奖项含金量。")
                      .arg(awardCount);
    } else {
        advice << QString("已获得 %1 项奖项，荣誉成果丰硕，建议整理获奖经历，用于升学申请和求职展示。")
                      .arg(awardCount);
    }

    return advice.join("\n\n");
}

QString AdvicePage::generateSummaryAdvice() const
{
    GpaCalculator &gpa = GpaCalculator::instance();
    double totalGpa = gpa.getTotalGpa();
    int courseCount = gpa.getCourseCount();
    int expCount = gpa.getExperienceCount();
    int awardCount = gpa.getAwardCount();
    bool hasIntern = gpa.hasInternship();

    QStringList priorities;

    if (courseCount == 0 || (courseCount > 0 && totalGpa < 2.5)) {
        priorities << "学业成绩提升";
    }
    if (expCount < 2 || !hasIntern) {
        priorities << "实践经历补充";
    }
    if (awardCount < 2) {
        priorities << "奖项荣誉积累";
    }

    if (priorities.isEmpty()) {
        return "综合来看，你在学业、实践和荣誉三个维度均有不错的表现。"
               "建议保持当前发展势头，并根据个人职业规划有针对性地深化某一方向，"
               "在实践中持续积累核心竞争力。";
    }

    return QString("下阶段重点发展方向：%1。建议合理规划时间，分阶段逐步提升各维度表现，优先补齐短板。")
        .arg(priorities.join("、"));
}
