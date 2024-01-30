#include "LiveRoomMsgItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "ui_LiveRoomMsgItem.h"

LiveRoomMsgItem::LiveRoomMsgItem(QWidget* parent) : QWidget(parent), ui(new Ui::LiveRoomMsgItem) {
    ui->setupUi(this);
}

LiveRoomMsgItem::~LiveRoomMsgItem() {
    delete ui;
}

void LiveRoomMsgItem::setAvatar(const QString& avatarUrl) {
    std::ignore = QtConcurrent::run([=]() {
        auto manager = new QNetworkAccessManager();
        QNetworkRequest request(avatarUrl);
        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data);
            ui->m_avatarLabel->setPixmap(pixmap);
        } else {
            qDebug() << "load failed: " << reply->errorString();
        }
    });
}

void LiveRoomMsgItem::setNicknameAndMsg(const QString& nickname, const QString& msg) {
    //    int oldHeight = ui->m_nicknameAndMsgLabel->height();
    QFontMetrics fontMetrics(ui->m_nicknameAndMsgLabel->font());
    QString elideText = fontMetrics.elidedText(QString("%1: %2").arg(nickname, msg), Qt::ElideRight,
                                               ui->m_nicknameAndMsgLabel->width());
    ui->m_nicknameAndMsgLabel->setText(elideText);
    //    ui->m_nicknameAndMsgLabel->setWordWrap(true);
    //    ui->m_nicknameAndMsgLabel->adjustSize();
    //    ui->m_nicknameAndMsgLabel->setMinimumWidth(195);
    //    ui->m_nicknameAndMsgLabel->setMaximumWidth(195);
    //    if(content.size() < 20) {
    //        ui->m_nicknameAndMsgLabel->setMinimumHeight(20);
    //        ui->m_nicknameAndMsgLabel->setMaximumHeight(20);
    //    }
    //    qDebug() << "ui->m_nicknameAndMsgLabel->height(): " << ui->m_nicknameAndMsgLabel->height();
    //    qDebug() << "ui->m_nicknameAndMsgLabel->width(): " << ui->m_nicknameAndMsgLabel->width();
    //    int newHeight = ui->m_nicknameAndMsgLabel->height();
    //    int deltaHeight = newHeight - oldHeight;
    //    int oldHeight2 = height();
    //    setMinimumHeight(oldHeight2 + deltaHeight);
    //    setMaximumHeight(oldHeight2 + deltaHeight);
    //    setFixedHeight(oldHeight2 + deltaHeight);
}
