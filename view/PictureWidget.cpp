#include "PictureWidget.hpp"

#include "ui_PictureWidget.h"

PictureWidget::PictureWidget(QWidget* parent) : QWidget(parent), ui(new Ui::PictureWidget) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

PictureWidget::~PictureWidget() {
    delete ui;
}

void PictureWidget::slots_setImage(const QImage& image) {
    if (image.isNull()) {
        return;
    }
    QPixmap pixmap = QPixmap::fromImage(image.scaled(ui->lb_showImage->size(), Qt::KeepAspectRatio));
    ui->lb_showImage->setPixmap(pixmap);
}
