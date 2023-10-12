#include "FriendItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "ui_FriendItem.h"

FriendItem::FriendItem(QWidget* parent) : QWidget(parent), ui(new Ui::FriendItem) {
    ui->setupUi(this);
}

void FriendItem::setAvatar(const QString& url) {
    auto manager = new QNetworkAccessManager();
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data);
            ui->m_avatarPushButton->setIcon(QIcon(pixmap));
            ui->m_avatarPushButton->setIconSize(ui->m_avatarPushButton->size());
            ui->m_avatarPushButton->setFlat(true);
        } else {
            qDebug() << "Download failed:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void FriendItem::setNickName(const QString& nickName) {
    ui->m_nicknameLabel->setText(nickName);
}

void FriendItem::setStatus(const QString& status) {
    if (status.size() == 5) {
        ui->m_statusLabel->setGeometry(60, 30, 46, 20);
        ui->m_feelingLabel->setGeometry(108, 30, 150, 20);
    } else if (status.size() == 6) {
        ui->m_statusLabel->setGeometry(60, 30, 56, 20);
        ui->m_feelingLabel->setGeometry(120, 30, 150, 20);
    }
    ui->m_statusLabel->setText(status);
}

void FriendItem::setFeeling(const QString& feeling) {
    ui->m_feelingLabel->setText(feeling);
}

FriendItem::~FriendItem() {
    delete ui;
}
