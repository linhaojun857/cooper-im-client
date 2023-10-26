#include "ImageViewWidget.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "ui_ImageViewWidget.h"

ImageViewWidget::ImageViewWidget(QWidget* parent) : QWidget(parent), ui(new Ui::ImageViewWidget) {
    ui->setupUi(this);
    setWindowTitle("Cooper");
    setWindowIcon(QIcon(":/img/logo.ico"));
}

void ImageViewWidget::setImage(const QString& url) {
    auto manager = new QNetworkAccessManager();
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data);
            ui->m_imageLabel->resize(this->size());
            ui->m_imageLabel->setPixmap(pixmap.scaled(this->size(), Qt::KeepAspectRatio));
        } else {
            qDebug() << "load failed: " << reply->errorString();
        }
    });
}

void ImageViewWidget::clearImage() {
    ui->m_imageLabel->clear();
}

ImageViewWidget::~ImageViewWidget() {
    delete ui;
}
