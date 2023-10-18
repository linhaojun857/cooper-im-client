#include "FGSWidget.hpp"

#include "ui_FGSWidget.h"

FGSWidget::FGSWidget(QWidget* parent) : QWidget(parent), ui(new Ui::FGSWidget) {
    ui->setupUi(this);
}

FGSWidget::~FGSWidget() {
    delete ui;
}
