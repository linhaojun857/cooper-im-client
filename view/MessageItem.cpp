#include "MessageItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "ui_MessageItem.h"

MessageItem::MessageItem(QWidget* parent) : QWidget(parent), ui(new Ui::MessageItem) {
    ui->setupUi(this);
}

MessageItem::~MessageItem() {
    delete ui;
}

void MessageItem::setId(int id) {
    m_id = id;
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
}
