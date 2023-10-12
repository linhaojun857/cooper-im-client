#include "GroupWidget.hpp"

#include "ui_GroupWidget.h"

GroupWidget::GroupWidget(QWidget* parent) : QWidget(parent), ui(new Ui::GroupWidget) {
    ui->setupUi(this);
}

GroupWidget::~GroupWidget() {
    delete ui;
}
