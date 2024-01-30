#include "VideoCallRequestDialog.hpp"

#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QScreen>
#include <QtConcurrent/QtConcurrent>

#include "core/IMKernel.hpp"
#include "define/IMDefine.hpp"
#include "store/IMStore.hpp"
#include "ui_VideoCallRequestDialog.h"

VideoCallRequestDialog::VideoCallRequestDialog(QWidget* parent) : QWidget(parent), ui(new Ui::VideoCallRequestDialog) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    move(screenGeometry.width() - width() - 5, screenGeometry.height() - height() - 45);

    connect(ui->m_acceptPushButton, &QPushButton::clicked, this, &VideoCallRequestDialog::handleClickAcceptPushButton);
    connect(ui->m_rejectPushButton, &QPushButton::clicked, this, &VideoCallRequestDialog::handleClickRejectPushButton);
}

VideoCallRequestDialog::~VideoCallRequestDialog() {
    delete ui;
}

void VideoCallRequestDialog::setPeerId(int peerId) {
    m_peerId = peerId;
    QString avatar = IMStore::getInstance()->getFriend(peerId)->avatar;
    QString nickname = IMStore::getInstance()->getFriend(peerId)->nickname;

    std::ignore = QtConcurrent::run([=]() {
        auto manager = new QNetworkAccessManager();
        QNetworkRequest request(avatar);
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

    QString msg = QString("%1发起了视频通话").arg(nickname);
    QFontMetrics fontMetrics(ui->m_msgLabel->font());
    QString elideText = fontMetrics.elidedText(msg, Qt::ElideRight, ui->m_msgLabel->width());
    ui->m_msgLabel->setText(elideText);
}

void VideoCallRequestDialog::handleClickAcceptPushButton() {
    QJsonObject json;
    json["token"] = IMStore::getInstance()->getToken();
    json["type"] = PROTOCOL_TYPE_VIDEO_CALL_RESPONSE;
    json["from_id"] = IMStore::getInstance()->getSelf()->id;
    json["to_id"] = m_peerId;
    json["code"] = 4;
    IMStore::getInstance()->getIMKernel()->sendVideoCallResponse(json);
    auto videoCallDialog = new VideoCallDialog();
    videoCallDialog->setPeerId(m_peerId);
    videoCallDialog->show();
    videoCallDialog->startCall();
    close();
}

void VideoCallRequestDialog::handleClickRejectPushButton() {
    QJsonObject json;
    json["token"] = IMStore::getInstance()->getToken();
    json["type"] = PROTOCOL_TYPE_VIDEO_CALL_RESPONSE;
    json["from_id"] = IMStore::getInstance()->getSelf()->id;
    json["to_id"] = m_peerId;
    json["code"] = 2;
    IMStore::getInstance()->getIMKernel()->sendVideoCallResponse(json);
    close();
}
