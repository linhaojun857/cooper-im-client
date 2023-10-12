#include "CotPWidget.hpp"

#include <QTabBar>

#include "ui_CotPWidget.h"

CotPWidget::CotPWidget(QWidget* parent) : QWidget(parent), ui(new Ui::CotPWidget) {
    ui->setupUi(this);
    setMinimumWidth(300);
    setMaximumWidth(300);
    setMinimumHeight(474);
    setMaximumHeight(474);
    m_friendWidget = new FriendWidget(ui->m_tabWidget->widget(0));
    m_friendWidget->setGeometry(0, 0, 300, 451);
    ui->m_tabWidget->tabBar()->hide();
    m_tabBtnMap[0] = ui->m_friendPushButton;
    m_tabBtnMap[1] = ui->m_groupPushButton;
    m_tabBtnMap[2] = ui->m_newFriPushButton;
    connect(ui->m_friendPushButton, &QPushButton::clicked, [this]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_tabWidget->setCurrentIndex(0);
        m_tabCurrentIndex = 0;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
    connect(ui->m_groupPushButton, &QPushButton::clicked, [this]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_tabWidget->setCurrentIndex(1);
        m_tabCurrentIndex = 1;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
    connect(ui->m_newFriPushButton, &QPushButton::clicked, [this]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_tabWidget->setCurrentIndex(2);
        m_tabCurrentIndex = 2;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
}

CotPWidget::~CotPWidget() {
    delete ui;
}