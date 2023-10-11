#include "PyqWidget.hpp"

#include "ui_PyqWidget.h"

PyqWidget::PyqWidget(QWidget* parent) : QWidget(parent), ui(new Ui::PyqWidget) {
    ui->setupUi(this);
}

PyqWidget::~PyqWidget() {
    delete ui;
}
