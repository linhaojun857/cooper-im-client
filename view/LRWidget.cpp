#include "LRWidget.hpp"

#include <QTabBar>

#include "ui_LRWidget.h"

LRWidget::LRWidget(QWidget* parent) : QWidget(parent), ui(new Ui::LRWidget) {
    ui->setupUi(this);
    this->setWindowTitle("Cooper");
    ui->m_lRTabWidget->tabBar()->hide();
}

LRWidget::~LRWidget() {
    delete ui;
}
