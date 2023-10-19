#include "FGSWidget.hpp"

#include <QTabBar>

#include "mock/Mock.hpp"
#include "ui_FGSWidget.h"

FGSWidget::FGSWidget(QWidget* parent) : QWidget(parent), ui(new Ui::FGSWidget) {
    Mock::fgsWidget = this;
    ui->setupUi(this);
    setWindowTitle("Cooper");
    setWindowIcon(QIcon(":/img/logo.ico"));
    ui->m_tabWidget->tabBar()->hide();
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
    ui->m_fScrollArea->setFrameStyle(QFrame::NoFrame);
    ui->m_fScrollArea->setFrameShape(QFrame::NoFrame);
    ui->m_fScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->m_fScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto fSRWidget = new QWidget();
    ui->m_fScrollArea->setWidget(fSRWidget);
    m_fsrItemsLayout = new QGridLayout();
    m_fsrItemsLayout->setContentsMargins(0, 0, 0, 0);
    m_fsrItemsLayout->setSpacing(0);
    fSRWidget->setLayout(m_fsrItemsLayout);
    ui->m_tabWidget->setCurrentIndex(m_tabCurrentIndex);
}

FGSWidget::~FGSWidget() {
    delete ui;
}

void FGSWidget::addFSRItem(FSRItem* fsrItem, int row, int column) {
    m_fsrItemsLayout->addWidget(fsrItem, row, column);
}
