#include "FGSWidget.hpp"

#include <QMessageBox>
#include <QTabBar>

#include "define/IMDefine.hpp"
#include "mock/Mock.hpp"
#include "store/IMStore.hpp"
#include "ui_FGSWidget.h"
#include "util/HttpUtil.hpp"

FGSWidget::FGSWidget(QWidget* parent) : QWidget(parent), ui(new Ui::FGSWidget) {
    Mock::fgsWidget = this;
    ui->setupUi(this);
    setWindowTitle("Cooper");
    setWindowIcon(QIcon(":/img/logo.ico"));
    ui->m_tabWidget->tabBar()->hide();
    ui->m_sLineEdit->setPlaceholderText("请输入账号/昵称");
    m_tabBtnMap[0] = ui->m_findFPushButton;
    m_tabBtnMap[1] = ui->m_findGPushButton;
    connect(ui->m_findFPushButton, &QPushButton::clicked, [this]() {
        m_searchMode = 0;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_tabWidget->setCurrentIndex(0);
        m_tabCurrentIndex = 0;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
    connect(ui->m_findGPushButton, &QPushButton::clicked, [this]() {
        m_searchMode = 1;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_tabWidget->setCurrentIndex(1);
        m_tabCurrentIndex = 1;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
    connect(ui->m_sPushButton, &QPushButton::clicked, this, &FGSWidget::handleUserClickSearchPushButton);
    ui->m_fScrollArea->setFrameStyle(QFrame::NoFrame);
    ui->m_fScrollArea->setFrameShape(QFrame::NoFrame);
    ui->m_fScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->m_fScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto fSRWidget = new QWidget();
    ui->m_fScrollArea->setWidget(fSRWidget);
    m_fsrItemsLayout = new QGridLayout();
    m_fsrItemsLayout->setContentsMargins(0, 0, 0, 0);
    m_fsrItemsLayout->setSpacing(0);
    m_fsrItemsLayout->setColumnMinimumWidth(0, 150);
    m_fsrItemsLayout->setColumnMinimumWidth(1, 150);
    m_fsrItemsLayout->setColumnMinimumWidth(2, 150);
    fSRWidget->setLayout(m_fsrItemsLayout);
    ui->m_tabWidget->setCurrentIndex(m_tabCurrentIndex);
}

FGSWidget::~FGSWidget() {
    delete ui;
}

void FGSWidget::clearAllFSRItems() {
    QLayoutItem* child;
    while ((child = m_fsrItemsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    m_fsrItems.clear();
}

void FGSWidget::addFSRItem(FSRItem* fsrItem, int row, int column) {
    m_fsrItemsLayout->addWidget(fsrItem, row, column, Qt::AlignLeft | Qt::AlignTop);
}

void FGSWidget::handleUserClickSearchPushButton() {
    qDebug() << "handleUserClickSearchPushButton";
    clearAllFSRItems();
    auto keyword = ui->m_sLineEdit->text();
    if (keyword.isEmpty()) {
        return;
    }
    if (m_searchMode == 0) {
        QJsonObject json;
        json.insert("keyword", keyword);
        json.insert("token", IMStore::getInstance()->getToken());
        auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/search", json);
        if (ret["code"].toInt() == 20000) {
            IMStore::getInstance()->addFSRs(ret);
        } else {
            QMessageBox::warning(this, "提示", ret["msg"].toString());
        }
    } else {
    }
}
