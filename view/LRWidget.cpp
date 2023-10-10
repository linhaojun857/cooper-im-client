#include "LRWidget.hpp"

#include <QTabBar>

#include "ui_LRWidget.h"

LRWidget::LRWidget(QWidget* parent) : QWidget(parent), ui(new Ui::LRWidget) {
    ui->setupUi(this);
    this->setWindowTitle("Cooper");
    ui->m_lRTabWidget->tabBar()->hide();
    connect(ui->m_registerPushButton, &QPushButton::clicked, this, [&]() {
        ui->m_lRTabWidget->setCurrentIndex(1);
    });
    connect(ui->m_loginPushButton_r, &QPushButton::clicked, this, [&]() {
        ui->m_lRTabWidget->setCurrentIndex(0);
    });
}

LRWidget::~LRWidget() {
    delete ui;
}
