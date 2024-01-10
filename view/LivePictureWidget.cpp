#include "LivePictureWidget.hpp"

#include "ui_LivePictureWidget.h"

LivePictureWidget::LivePictureWidget(QWidget* parent) : QWidget(parent), ui(new Ui::LivePictureWidget) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

LivePictureWidget::~LivePictureWidget() {
    delete ui;
}

void LivePictureWidget::slots_setImage(const QImage& image) {
    if (image.isNull()) {
        return;
    }
    QPixmap pixmap = QPixmap::fromImage(image.scaled(ui->lb_showImage->size(), Qt::KeepAspectRatio));
    ui->lb_showImage->setPixmap(pixmap);
}
