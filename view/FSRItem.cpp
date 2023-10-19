#include "FSRItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "ui_FSRItem.h"

FSRItem::FSRItem(QWidget* parent) : QWidget(parent), ui(new Ui::FSRItem) {
    ui->setupUi(this);
}

FSRItem::~FSRItem() {
    delete ui;
}

void FSRItem::setAvatar(const QString& url) {
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

QString FSRItem::getAvatar() {
    return m_avatarUrl;
}

void FSRItem::setNickname(const QString& nickname) {
    m_nickname = nickname;
    QFontMetrics fontMetrics(ui->m_nicknameLabel->font());
    QString elideText = fontMetrics.elidedText(nickname, Qt::ElideRight, ui->m_nicknameLabel->width());
    ui->m_nicknameLabel->setText(elideText);
}

QString FSRItem::getNickname() {
    return m_nickname;
}
