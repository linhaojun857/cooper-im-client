#include "FriendItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "define/IMDefine.hpp"
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

void FriendItem::setNickName(const QString& nickname) {
    m_nickName = nickname;
    QFontMetrics fontMetrics(ui->m_nicknameLabel->font());
    QString elideText = fontMetrics.elidedText(nickname, Qt::ElideRight, ui->m_nicknameLabel->width());
    ui->m_nicknameLabel->setText(elideText);
}

void FriendItem::setStatusAndFeeling(const QString& status, const QString& feeling) {
    if (!status.isEmpty()) {
        m_status = status;
    } else {
        m_status = DEFAULT_USER_STATUS_ONLINE;
    }
    m_feeling = feeling;
    QFontMetrics fontMetrics(ui->m_statusAndFeelingLabel->font());
    QString elideText = fontMetrics.elidedText(QString("[%1] %2").arg(m_status, m_feeling), Qt::ElideRight,
                                               ui->m_statusAndFeelingLabel->width());
    ui->m_statusAndFeelingLabel->setText(elideText);
}

FriendItem::~FriendItem() {
    delete ui;
}
