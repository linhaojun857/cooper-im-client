#include "MainWidget.hpp"

#include <QLayout>
#include <QTabBar>

#include "ui_MainWidget.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    setWindowTitle("Cooper");
    setWindowIcon(QIcon(":/img/logo.ico"));
    m_cotPWidget = new CotPWidget(ui->m_tabWidget->widget(1));
    m_cotPWidget->setGeometry(0, 0, 300, 474);
    setMinimumWidth(300);
    setMaximumWidth(300);
    setMinimumHeight(600);
    setMaximumHeight(600);
    ui->m_tabWidget->tabBar()->hide();
    m_tabBtnMap[0] = ui->m_msgPushButton;
    m_tabBtnMap[1] = ui->m_cotPPushButton;
    m_tabBtnMap[2] = ui->m_pyqPushButton;
    connect(ui->m_msgPushButton, &QPushButton::clicked, this, [&]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_tabWidget->setCurrentIndex(0);
        m_tabCurrentIndex = 0;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
    connect(ui->m_cotPPushButton, &QPushButton::clicked, this, [&]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_tabWidget->setCurrentIndex(1);
        m_tabCurrentIndex = 1;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
    connect(ui->m_pyqPushButton, &QPushButton::clicked, this, [&]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_tabWidget->setCurrentIndex(2);
        m_tabCurrentIndex = 2;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
}

MainWidget::~MainWidget() {
    delete ui;
}
