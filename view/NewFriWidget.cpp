#include "NewFriWidget.hpp"

#include "ui_NewFriWidget.h"

NewFriWidget::NewFriWidget(QWidget* parent) : QWidget(parent), ui(new Ui::NewFriWidget) {
    ui->setupUi(this);
}

NewFriWidget::~NewFriWidget() {
    delete ui;
}
