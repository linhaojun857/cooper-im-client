#include "NewFriendItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "ui_NewFriendItem.h"

NewFriendItem::NewFriendItem(QWidget* parent) : QWidget(parent), ui(new Ui::NewFriendItem) {
    ui->setupUi(this);
}

NewFriendItem::~NewFriendItem() {
    delete ui;
}

void NewFriendItem::setAvatar(const QString& url) {
    m_avatarUrl = url;
    std::ignore = QtConcurrent::run([=]() {
        auto manager = new QNetworkAccessManager();
        QNetworkRequest request(url);
        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data);
            ui->m_avatarPushButton->setIcon(QIcon(pixmap));
            ui->m_avatarPushButton->setIconSize(ui->m_avatarPushButton->size());
            ui->m_avatarPushButton->setFlat(true);
        } else {
            qDebug() << "load failed: " << reply->errorString();
        }
    });
}

void NewFriendItem::setNickname(const QString& nickname) {
    m_nickname = nickname;
    ui->m_nicknameLabel->setText(nickname);
}

void NewFriendItem::setReason(const QString& reason) {
    m_reason = reason;
    QFontMetrics fontMetrics(ui->m_reasonLabel->font());
    QString elideText = fontMetrics.elidedText(reason, Qt::ElideRight, ui->m_reasonLabel->width());
    ui->m_reasonLabel->setText(elideText);
}
