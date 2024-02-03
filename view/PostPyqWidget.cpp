#include "PostPyqWidget.hpp"

#include "ui_PostPyqWidget.h"

PostPyqWidget::PostPyqWidget(QWidget* parent) : QWidget(parent), ui(new Ui::PostPyqWidget) {
    ui->setupUi(this);
}

PostPyqWidget::~PostPyqWidget() {
    delete ui;
}
