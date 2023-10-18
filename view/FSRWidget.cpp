#include "FSRWidget.hpp"

#include "ui_FSRWidget.h"

FSRWidget::FSRWidget(QWidget* parent) : QWidget(parent), ui(new Ui::FSRWidget) {
    ui->setupUi(this);
}

FSRWidget::~FSRWidget() {
    delete ui;
}
