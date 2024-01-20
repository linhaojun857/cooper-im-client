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
    if (m_adjustFlag) {
        double ratio = image.height() * 1.0 / image.width();
        double height = ratio * ui->lb_showImage->width();
        if (height > ui->lb_showImage->height()) {
            height = ui->lb_showImage->height();
        }
        ui->lb_showImage->setFixedSize(ui->lb_showImage->width(), (int)height);
        ui->lb_showImage->move((ui->lb_showImage->parentWidget()->width() - ui->lb_showImage->width()) / 2,
                               (ui->lb_showImage->parentWidget()->height() - ui->lb_showImage->height()) / 2);
        m_adjustFlag = false;
    }
    ui->lb_showImage->setPixmap(QPixmap::fromImage(image));
}

void LivePictureWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->globalPosition() - frameGeometry().topLeft();
        m_isDragging = true;
    }
}

void LivePictureWidget::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton && m_isDragging) {
        auto pos = event->globalPosition() - m_dragStartPosition;
        move(pos.toPoint());
    }
}

void LivePictureWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
}
