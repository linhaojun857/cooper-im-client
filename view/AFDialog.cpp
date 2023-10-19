#include "AFDialog.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "ui_AFDialog.h"

AFDialog::AFDialog(QWidget* parent) : QWidget(parent), ui(new Ui::AFDialog) {
    ui->setupUi(this);
    ui->m_viTextEdit->setPlaceholderText("我是...");
}

AFDialog::~AFDialog() {
    delete ui;
}

void AFDialog::setAvatar(const QString& url) {
    m_avatarUrl = url;
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
        QIcon icon(pixmap);
        setWindowIcon(icon);
    } else {
        qDebug() << "load failed: " << reply->errorString();
    }
}

QString AFDialog::getAvatar() {
    return m_avatarUrl;
}

void AFDialog::setNickname(const QString& nickname) {
    setWindowTitle(nickname);
}

QString AFDialog::getNickname() {
    return m_nickname;
}
