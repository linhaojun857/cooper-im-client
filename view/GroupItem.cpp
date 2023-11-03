#include "GroupItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "ui_GroupItem.h"

GroupItem::GroupItem(QWidget* parent) : QWidget(parent), ui(new Ui::GroupItem) {
    ui->setupUi(this);
}

GroupItem::~GroupItem() {
    delete ui;
}

void GroupItem::setId(int id) {
    m_id = id;
}

int GroupItem::getId() const {
    return m_id;
}

void GroupItem::setAvatar(const QString& url) {
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

QString GroupItem::getAvatar() {
    return m_avatarUrl;
}

void GroupItem::setName(const QString& groupName) {
    m_groupName = groupName;
    QFontMetrics fontMetrics(ui->m_groupNameLabel->font());
    QString elideText = fontMetrics.elidedText(groupName, Qt::ElideRight, ui->m_groupNameLabel->width());
    ui->m_groupNameLabel->setText(elideText);
}

QString GroupItem::getGroupName() {
    return m_groupName;
}
