#include "awardpage.h"
#include "gpacalculator.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

Awardpage::Awardpage(QWidget *parent)
    : QWidget(parent)
{
    // -------------------------- 顶部输入区 --------------------------
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->setSpacing(10);
    inputLayout->setContentsMargins(10, 10, 10, 10);

    inputLayout->addWidget(new QLabel("奖项名称"));
    m_editTitle = new QLineEdit;
    m_editTitle->setPlaceholderText("如 国家奖学金");
    inputLayout->addWidget(m_editTitle, 1);

    inputLayout->addWidget(new QLabel("级别"));
    m_comboLevel = new QComboBox;
    m_comboLevel->addItems({"国家级", "省级", "校级", "院级"});
    inputLayout->addWidget(m_comboLevel);

    inputLayout->addWidget(new QLabel("日期"));
    m_editDate = new QLineEdit;
    m_editDate->setPlaceholderText("如 2024-10");
    inputLayout->addWidget(m_editDate);

    m_btnAdd = new QPushButton("添加");
    m_btnAdd->setFixedWidth(80);
    inputLayout->addWidget(m_btnAdd);

    // -------------------------- 描述输入行 --------------------------
    QHBoxLayout *descLayout = new QHBoxLayout;
    descLayout->setContentsMargins(10, 0, 10, 10);
    descLayout->addWidget(new QLabel("描述"));
    m_editDesc = new QLineEdit;
    m_editDesc->setPlaceholderText("简要描述获奖情况");
    descLayout->addWidget(m_editDesc, 1);

    // -------------------------- 中间表格区 --------------------------
    m_tableAward = new QTableWidget;
    m_tableAward->setColumnCount(4);
    m_tableAward->setHorizontalHeaderLabels({"奖项名称", "级别", "日期", "描述"});
    m_tableAward->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableAward->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableAward->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // -------------------------- 底部操作 --------------------------
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->setContentsMargins(10, 10, 10, 10);
    m_btnDelete = new QPushButton("删除选中奖项");
    m_btnDelete->setFixedWidth(140);
    bottomLayout->addWidget(m_btnDelete);
    bottomLayout->addStretch();

    // -------------------------- 主布局 --------------------------
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(descLayout);
    mainLayout->addWidget(m_tableAward, 1);
    mainLayout->addLayout(bottomLayout);

    // 绑定事件
    connect(m_btnAdd, &QPushButton::clicked, this, &Awardpage::addAward);
    connect(m_btnDelete, &QPushButton::clicked, this, &Awardpage::deleteAward);

    // 启动时加载已有数据
    loadAwards();
}

void Awardpage::loadAwards()
{
    QSqlQuery query = GpaCalculator::instance().getAllAwards();
    m_tableAward->setRowCount(0);
    while (query.next()) {
        int id = query.value("id").toInt();
        QString title = query.value("title").toString();
        QString level = query.value("level").toString();
        QString date = query.value("date").toString();
        QString desc = query.value("description").toString();

        int row = m_tableAward->rowCount();
        m_tableAward->insertRow(row);

        QTableWidgetItem *titleItem = new QTableWidgetItem(title);
        titleItem->setData(Qt::UserRole, id);
        m_tableAward->setItem(row, 0, titleItem);
        m_tableAward->setItem(row, 1, new QTableWidgetItem(level));
        m_tableAward->setItem(row, 2, new QTableWidgetItem(date));
        m_tableAward->setItem(row, 3, new QTableWidgetItem(desc));
    }
}

void Awardpage::addAward()
{
    QString title = m_editTitle->text().trimmed();
    QString level = m_comboLevel->currentText();
    QString date = m_editDate->text().trimmed();
    QString desc = m_editDesc->text().trimmed();

    if (title.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入奖项名称");
        return;
    }

    if (!GpaCalculator::instance().addAward(title, level, date, desc)) {
        QMessageBox::warning(this, "错误", "保存到数据库失败");
        return;
    }

    m_editTitle->clear();
    m_editDate->clear();
    m_editDesc->clear();

    loadAwards();
}

void Awardpage::deleteAward()
{
    int row = m_tableAward->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "提示", "请先选择要删除的奖项");
        return;
    }

    QTableWidgetItem *item = m_tableAward->item(row, 0);
    int id = item->data(Qt::UserRole).toInt();
    if (id <= 0) return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除",
        QString("确定要删除奖项 \"%1\" 吗？").arg(item->text()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        GpaCalculator::instance().deleteAward(id);
        loadAwards();
    }
}
