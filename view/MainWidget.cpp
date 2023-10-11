#include "MainWidget.hpp"

#include <QSplitter>
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
    ui->m_listTabWidget->tabBar()->hide();
    connect(ui->m_msgPushButton, &QPushButton::clicked, this, [&]() {
        ui->m_listTabWidget->setCurrentIndex(0);
    });
    connect(ui->m_friendPushButton, &QPushButton::clicked, this, [&]() {
        ui->m_listTabWidget->setCurrentIndex(1);
    });
    connect(ui->m_pyqPushButton, &QPushButton::clicked, this, [&]() {
        ui->m_listTabWidget->setCurrentIndex(2);
    });
}

MainWidget::~MainWidget() {
    delete ui;
}
