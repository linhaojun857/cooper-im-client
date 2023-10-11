#include "MainWidget.hpp"

#include <QTabBar>

#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    setWindowTitle("Cooper");
    setWindowIcon(QIcon(":/img/logo.ico"));
    setMinimumWidth(294);
    setMaximumWidth(294);
    setMinimumHeight(600);
    setMaximumHeight(600);
    ui->m_infoTabWidget->tabBar()->hide();
    ui->m_listTabWidget->tabBar()->hide();
    m_tabBtnMap[0] = ui->m_msgPushButton;
    m_tabBtnMap[1] = ui->m_friendPushButton;
    m_tabBtnMap[2] = ui->m_pyqPushButton;
    connect(ui->m_msgPushButton, &QPushButton::clicked, this, [&]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_listTabWidget->setCurrentIndex(0);
        m_tabCurrentIndex = 0;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
    connect(ui->m_friendPushButton, &QPushButton::clicked, this, [&]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_listTabWidget->setCurrentIndex(1);
        m_tabCurrentIndex = 1;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
    connect(ui->m_pyqPushButton, &QPushButton::clicked, this, [&]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_listTabWidget->setCurrentIndex(2);
        m_tabCurrentIndex = 2;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
}

MainWidget::~MainWidget() {
    delete ui;
}
