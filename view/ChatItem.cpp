#include "ChatItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPalette>
#include <QtConcurrent/QtConcurrent>

#include "store/IMStore.hpp"
#include "ui_ChatItem.h"

ChatItem::ChatItem(QWidget* parent) : QWidget(parent), ui(new Ui::ChatItem) {
    ui->setupUi(this);
    connect(ui->m_avatarPushButton, &QPushButton::clicked, [this]() {
        if (m_mode == 0) {
            IMStore::getInstance()->getChatDialog()->changeChatHistory(m_id, 0);
        } else {
            IMStore::getInstance()->getChatDialog()->changeChatHistory(m_id, 1);
        }
    });
}

ChatItem::~ChatItem() {
    delete ui;
}

void ChatItem::setMode(int mode) {
    m_mode = mode;
}

void ChatItem::setId(int id) {
    m_id = id;
}

int ChatItem::getId() const {
    return m_id;
}

void ChatItem::setAvatar(const QString& url) {
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

QString ChatItem::getAvatar() {
    return m_avatarUrl;
}

void ChatItem::setName(const QString& name) {
    m_name = name;
    ui->m_nameLabel->setText(name);
}

QString ChatItem::getName() {
    return m_name;
}

void ChatItem::setRecentMsg(const QString& recentMsg) {
    auto temp = recentMsg;
    m_recentMsg = temp.remove('\n');
    QFontMetrics fontMetrics(ui->m_recentMsgLabel->font());
    QString elideText = fontMetrics.elidedText(m_recentMsg, Qt::ElideRight, ui->m_recentMsgLabel->width());
    ui->m_recentMsgLabel->setText(elideText);
}
