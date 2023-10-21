#include "FSRItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "ui_FSRItem.h"

FSRItem::FSRItem(QWidget* parent) : QWidget(parent), ui(new Ui::FSRItem) {
    ui->setupUi(this);
    m_dialog = new AFDialog();
    connect(ui->m_addPushButton, &QPushButton::clicked, [this]() {
        m_dialog->setId(m_id);
        m_dialog->setAvatar(m_avatarUrl);
        m_dialog->setNickname(m_nickname);
        m_dialog->show();
    });
}

FSRItem::~FSRItem() {
    delete ui;
    delete m_dialog;
}

void FSRItem::setId(int id) {
    m_id = id;
}

int FSRItem::getId() const {
    return m_id;
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
