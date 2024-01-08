#include "LiveRoomItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "ui_LiveRoomItem.h"

LiveRoomItem::LiveRoomItem(QWidget* parent) : QWidget(parent), ui(new Ui::LiveRoomItem) {
    ui->setupUi(this);
    ui->m_liveRoomCoverLabel->installEventFilter(this);
    connect(this, &LiveRoomItem::liveRoomCoverClicked, this, &LiveRoomItem::handleLiveRoomCoverClicked);
}

LiveRoomItem::~LiveRoomItem() {
    delete ui;
}

bool LiveRoomItem::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->m_liveRoomCoverLabel) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto* mouseEvent = dynamic_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                emit liveRoomCoverClicked();
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void LiveRoomItem::setLiveRoomId(int liveRoomId) {
    m_liveRoomId = liveRoomId;
}

void LiveRoomItem::setLiveRoomOwnerName(const QString& liveRoomOwnerName) {
    m_liveRoomOwnerName = liveRoomOwnerName;
    ui->m_liveRoomOwnerNameLabel->setText(liveRoomOwnerName);
}

void LiveRoomItem::setLiveRoomCover(const QString& liveRoomCoverUrl) {
    m_liveRoomCoverUrl = liveRoomCoverUrl;
    std::ignore = QtConcurrent::run([=]() {
        auto manager = new QNetworkAccessManager();
        QNetworkRequest request(liveRoomCoverUrl);
        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data);
            ui->m_liveRoomCoverLabel->setPixmap(pixmap);
        } else {
            qDebug() << "load failed: " << reply->errorString();
        }
    });
}

void LiveRoomItem::setLiveRoomOwnerAvatar(const QString& liveRoomOwnerAvatarUrl) {
    m_liveRoomOwnerAvatarUrl = liveRoomOwnerAvatarUrl;
    std::ignore = QtConcurrent::run([=]() {
        auto manager = new QNetworkAccessManager();
        QNetworkRequest request(liveRoomOwnerAvatarUrl);
        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data);
            ui->m_liveRoomOwnerAvatarLabel->setPixmap(pixmap);
        } else {
            qDebug() << "load failed: " << reply->errorString();
        }
    });
}

void LiveRoomItem::setLiveRoomViewerCount(int liveRoomViewerCount) {
    m_liveRoomViewerCount = liveRoomViewerCount;
    ui->m_liveRoomViewerCountLabel->setText("观看人数: " + QString::number(liveRoomViewerCount));
}

void LiveRoomItem::handleLiveRoomCoverClicked() {
}
