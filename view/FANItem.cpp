#include "FANItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "ui_FANItem.h"

FANItem::FANItem(QWidget* parent) : QWidget(parent), ui(new Ui::FANItem) {
    ui->setupUi(this);
}

FANItem::~FANItem() {
    delete ui;
}

void FANItem::setMode(int mode) {
    m_mode = mode;
    if (mode == 0) {
        ui->m_agreePushButton->hide();
        ui->m_rejectPushButton->hide();
    } else {
        ui->m_statusLabel->hide();
    }
}

void FANItem::setAvatar(const QString& url) {
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

void FANItem::setNickname(const QString& nickname) {
    m_nickname = nickname;
    ui->m_nicknameLabel->setText(nickname);
}

void FANItem::setReason(const QString& reason) {
    m_reason = reason;
    QFontMetrics fontMetrics(ui->m_reasonLabel->font());
    QString elideText = fontMetrics.elidedText(reason, Qt::ElideRight, ui->m_reasonLabel->width());
    ui->m_reasonLabel->setText(elideText);
}
