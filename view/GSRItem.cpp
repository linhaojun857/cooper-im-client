#include "GSRItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "ui_GSRItem.h"

GSRItem::GSRItem(QWidget* parent) : QWidget(parent), ui(new Ui::GSRItem) {
    ui->setupUi(this);
    connect(ui->m_addPushButton, &QPushButton::clicked, this, &GSRItem::handleUserClickAddPushButton);
}

GSRItem::~GSRItem() {
    delete ui;
}

void GSRItem::setId(int id) {
    m_id = id;
}

void GSRItem::setAvatar(const QString& url) {
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

void GSRItem::setName(const QString& name) {
    QFontMetrics fontMetrics(ui->m_nameLabel->font());
    QString elideText = fontMetrics.elidedText(name, Qt::ElideRight, ui->m_nameLabel->width());
    ui->m_nameLabel->setText(elideText);
}

void GSRItem::setMemberCount(int memberCount) {
    ui->m_memberCountLabel->setText(QString::number(memberCount) + "人");
}

void GSRItem::handleUserClickAddPushButton() {
}
