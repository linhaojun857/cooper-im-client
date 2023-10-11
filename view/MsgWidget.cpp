#include "MsgWidget.hpp"

#include "ui_MsgWidget.h"

MsgWidget::MsgWidget(QWidget* parent) : QWidget(parent), ui(new Ui::MsgWidget) {
    ui->setupUi(this);
}

MsgWidget::~MsgWidget() {
    delete ui;
}
