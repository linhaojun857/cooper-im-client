#include "ChatItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPalette>

#include "store/IMStore.hpp"
#include "ui_ChatItem.h"

ChatItem::ChatItem(QWidget* parent) : QWidget(parent), ui(new Ui::ChatItem) {
    ui->setupUi(this);
    connect(ui->m_avatarPushButton, &QPushButton::clicked, [this]() {
        IMStore::getInstance()->getChatDialog()->changeChatHistory(m_id);
    });
}

ChatItem::~ChatItem() {
    delete ui;
}

void ChatItem::setId(int id) {
    m_id = id;
}

int ChatItem::getId() const {
    return m_id;
}

void ChatItem::setAvatar(const QString& url) {
    m_avatar = url;
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
            qDebug() << "load failed: " << reply->errorString();
        }
        reply->deleteLater();
    });
}

QString ChatItem::getAvatar() {
    return m_avatar;
}

void ChatItem::setName(const QString& name) {
    m_name = name;
    ui->m_nameLabel->setText(name);
}

QString ChatItem::getName() {
    return m_name;
}
