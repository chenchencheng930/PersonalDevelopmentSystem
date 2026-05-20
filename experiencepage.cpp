#include "experiencepage.h"
#include "gpacalculator.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

Experiencepage::Experiencepage(QWidget *parent)
    : QWidget(parent)
{
    // -------------------------- 顶部输入区 --------------------------
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->setSpacing(10);
    inputLayout->setContentsMargins(10, 10, 10, 10);

    inputLayout->addWidget(new QLabel("类型"));
    m_comboType = new QComboBox;
    m_comboType->addItems({"竞赛", "项目", "实习", "志愿者", "班级代表", "课外活动"});
    inputLayout->addWidget(m_comboType);

    inputLayout->addWidget(new QLabel("标题"));
    m_editTitle = new QLineEdit;
    m_editTitle->setPlaceholderText("活动/竞赛名称");
    inputLayout->addWidget(m_editTitle, 1);

    inputLayout->addWidget(new QLabel("日期"));
    m_editDate = new QLineEdit;
    m_editDate->setPlaceholderText("如 2024-06");
    inputLayout->addWidget(m_editDate);

    inputLayout->addWidget(new QLabel("角色"));
    m_editRole = new QLineEdit;
    m_editRole->setPlaceholderText("如 队长/成员");
    inputLayout->addWidget(m_editRole);

    m_btnAdd = new QPushButton("添加");
    m_btnAdd->setFixedWidth(80);
    inputLayout->addWidget(m_btnAdd);

    // -------------------------- 描述输入行 --------------------------
    QHBoxLayout *descLayout = new QHBoxLayout;
    descLayout->setContentsMargins(10, 0, 10, 10);
    descLayout->addWidget(new QLabel("描述"));
    m_editDesc = new QLineEdit;
    m_editDesc->setPlaceholderText("简要描述经历内容与收获");
    descLayout->addWidget(m_editDesc, 1);

    // -------------------------- 中间表格区 --------------------------
    m_tableExperience = new QTableWidget;
    m_tableExperience->setColumnCount(5);
    m_tableExperience->setHorizontalHeaderLabels({"类型", "标题", "日期", "角色", "描述"});
    m_tableExperience->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableExperience->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableExperience->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // -------------------------- 底部操作 --------------------------
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->setContentsMargins(10, 10, 10, 10);
    m_btnDelete = new QPushButton("删除选中经历");
    m_btnDelete->setFixedWidth(140);
    bottomLayout->addWidget(m_btnDelete);
    bottomLayout->addStretch();

    // -------------------------- 主布局 --------------------------
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(descLayout);
    mainLayout->addWidget(m_tableExperience, 1);
    mainLayout->addLayout(bottomLayout);

    // 绑定事件
    connect(m_btnAdd, &QPushButton::clicked, this, &Experiencepage::addExperience);
    connect(m_btnDelete, &QPushButton::clicked, this, &Experiencepage::deleteExperience);

    // 启动时加载已有数据
    loadExperiences();
}

void Experiencepage::loadExperiences()
{
    QSqlQuery query = GpaCalculator::instance().getAllExperiences();
    m_tableExperience->setRowCount(0);
    while (query.next()) {
        int id = query.value("id").toInt();
        QString type = query.value("type").toString();
        QString title = query.value("title").toString();
        QString date = query.value("date").toString();
        QString role = query.value("role").toString();
        QString desc = query.value("description").toString();

        int row = m_tableExperience->rowCount();
        m_tableExperience->insertRow(row);

        QTableWidgetItem *typeItem = new QTableWidgetItem(type);
        typeItem->setData(Qt::UserRole, id);
        m_tableExperience->setItem(row, 0, typeItem);
        m_tableExperience->setItem(row, 1, new QTableWidgetItem(title));
        m_tableExperience->setItem(row, 2, new QTableWidgetItem(date));
        m_tableExperience->setItem(row, 3, new QTableWidgetItem(role));
        m_tableExperience->setItem(row, 4, new QTableWidgetItem(desc));
    }
}

void Experiencepage::addExperience()
{
    QString type = m_comboType->currentText();
    QString title = m_editTitle->text().trimmed();
    QString date = m_editDate->text().trimmed();
    QString role = m_editRole->text().trimmed();
    QString desc = m_editDesc->text().trimmed();

    if (title.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入经历标题");
        return;
    }

    if (!GpaCalculator::instance().addExperience(type, title, date, role, desc)) {
        QMessageBox::warning(this, "错误", "保存到数据库失败");
        return;
    }

    m_editTitle->clear();
    m_editDate->clear();
    m_editRole->clear();
    m_editDesc->clear();

    loadExperiences();
}

void Experiencepage::deleteExperience()
{
    int row = m_tableExperience->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "提示", "请先选择要删除的经历");
        return;
    }

    QTableWidgetItem *item = m_tableExperience->item(row, 0);
    int id = item->data(Qt::UserRole).toInt();
    if (id <= 0) return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除",
        QString("确定要删除经历 \"%1\" 吗？").arg(m_tableExperience->item(row, 1)->text()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        GpaCalculator::instance().deleteExperience(id);
        loadExperiences();
    }
}
