#include "FriendItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "store/IMStore.hpp"
#include "ui_FriendItem.h"
#include "view/ChatItem.hpp"

FriendItem::FriendItem(QWidget* parent) : QWidget(parent), ui(new Ui::FriendItem) {
    ui->setupUi(this);
    connect(ui->m_avatarPushButton, &QPushButton::clicked, [this]() {
        if (!IMStore::getInstance()->getChatDialog()->isVisible()) {
            IMStore::getInstance()->getChatDialog()->show();
        }
        if (!IMStore::getInstance()->isOpenChatPage(m_id)) {
            qDebug() << "open chat page";
            IMStore::getInstance()->openChatPage(m_id);
            auto chatItem = new ChatItem();
            chatItem->setId(m_id);
            chatItem->setAvatar(m_avatarUrl);
            chatItem->setName(m_nickName);
            IMStore::getInstance()->getChatDialog()->addChatItem(chatItem);
        }
    });
}

void FriendItem::setId(int id) {
    m_id = id;
}

void FriendItem::setAvatar(const QString& url) {
    m_avatarUrl = url;
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

void FriendItem::setNickName(const QString& nickname) {
    m_nickName = nickname;
    QFontMetrics fontMetrics(ui->m_nicknameLabel->font());
    QString elideText = fontMetrics.elidedText(nickname, Qt::ElideRight, ui->m_nicknameLabel->width());
    ui->m_nicknameLabel->setText(elideText);
}

void FriendItem::setStatus(const QString& status) {
    m_status = status;
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
    m_feeling = feeling;
    QFontMetrics fontMetrics(ui->m_feelingLabel->font());
    QString elideText = fontMetrics.elidedText(feeling, Qt::ElideRight, ui->m_feelingLabel->width());
    ui->m_feelingLabel->setText(elideText);
}

FriendItem::~FriendItem() {
    delete ui;
}
