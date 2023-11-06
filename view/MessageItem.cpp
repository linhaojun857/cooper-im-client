#include "MessageItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "store/IMStore.hpp"
#include "ui_MessageItem.h"

MessageItem::MessageItem(QWidget* parent) : QWidget(parent), ui(new Ui::MessageItem) {
    ui->setupUi(this);
    connect(ui->m_avatarPushButton, &QPushButton::clicked, [this]() {
        if (!IMStore::getInstance()->getChatDialog()->isVisible()) {
            IMStore::getInstance()->getChatDialog()->showDialog();
        }
        if (m_mode == 0) {
            if (!IMStore::getInstance()->isOpenPersonChatPage(m_id)) {
                IMStore::getInstance()->openPersonChatPage(m_id);
                IMStore::getInstance()->getChatDialog()->addChatItem(m_id, 0);
                IMStore::getInstance()->getChatDialog()->changeChatHistory(m_id, 0);
            }
        } else {
            if (!IMStore::getInstance()->isOpenGroupChatPage(m_id)) {
                IMStore::getInstance()->openGroupChatPage(m_id);
                IMStore::getInstance()->getChatDialog()->addChatItem(m_id, 1);
                IMStore::getInstance()->getChatDialog()->changeChatHistory(m_id, 1);
            }
        }
    });
}

MessageItem::~MessageItem() {
    delete ui;
}

void MessageItem::setMode(int mode) {
    m_mode = mode;
}

void MessageItem::setId(int id) {
    m_id = id;
}

int MessageItem::getId() const {
    return m_id;
}

void MessageItem::setAvatar(const QString& url) {
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

void MessageItem::setName(const QString& name) {
    m_name = name;
}

void MessageItem::setRecentMsg(const QString& recentMsg) {
    auto temp = recentMsg;
    m_recentMsg = temp.remove('\n');
    QFontMetrics fontMetrics(ui->m_recentMsgLabel->font());
    QString elideText = fontMetrics.elidedText(m_recentMsg, Qt::ElideRight, ui->m_recentMsgLabel->width());
    ui->m_recentMsgLabel->setText(elideText);
}

void MessageItem::setTime(long long timestamp) {
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime messageTime = QDateTime::fromSecsSinceEpoch(timestamp);

    if (currentTime.date() == messageTime.date()) {
        m_time = messageTime.toString("hh:mm");
    } else {
        QDateTime yesterday = currentTime.addDays(-1);
        if (yesterday.date() == messageTime.date()) {
            m_time = "昨天 " + messageTime.toString("hh:mm");
        } else if (currentTime.date().year() == messageTime.date().year()) {
            m_time = messageTime.toString("MM-dd");
        } else {
            m_time = messageTime.toString("yy-MM-dd");
        }
    }
    ui->m_timeLabel->setText(m_time);
}

void MessageItem::debug() {
    qDebug() << "m_id: " << m_id;
    qDebug() << "m_avatarUrl: " << m_avatarUrl;
    qDebug() << "m_name: " << m_name;
    qDebug() << "m_recentMsg: " << m_recentMsg;
    qDebug() << "m_time: " << m_time;
}
