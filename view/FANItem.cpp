#include "FANItem.hpp"

#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "define/IMDefine.hpp"
#include "store/IMStore.hpp"
#include "ui_FANItem.h"
#include "util/HttpUtil.hpp"

FANItem::FANItem(QWidget* parent) : QWidget(parent), ui(new Ui::FANItem) {
    ui->setupUi(this);
    connect(ui->m_agreePushButton, &QPushButton::clicked, std::bind(&FANItem::responseFriendApply, this, 1));
    connect(ui->m_rejectPushButton, &QPushButton::clicked, std::bind(&FANItem::responseFriendApply, this, 2));
}

FANItem::~FANItem() {
    delete ui;
}

void FANItem::setMode(int mode) {
    m_mode = mode;
    if (mode == 0) {
        ui->m_statusLabel->show();
        ui->m_agreePushButton->hide();
        ui->m_rejectPushButton->hide();
    } else {
        ui->m_statusLabel->hide();
        ui->m_agreePushButton->show();
        ui->m_rejectPushButton->show();
    }
}

void FANItem::setFromId(int fromId) {
    m_fromId = fromId;
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

void FANItem::setStatus(const QString& status) {
    ui->m_statusLabel->setText(status);
}

void FANItem::responseFriendApply(int agree) {
    QJsonObject json;
    json["from_id"] = m_fromId;
    json["agree"] = agree;
    json["token"] = IMStore::getInstance()->getToken();
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/responseFriendApply", json);
    if (ret["code"] == HTTP_SUCCESS_CODE) {
        ui->m_agreePushButton->hide();
        ui->m_rejectPushButton->hide();
        ui->m_statusLabel->show();
        if (agree == 1) {
            ui->m_statusLabel->setText("已同意");
        } else {
            ui->m_statusLabel->setText("已拒绝");
        }
    } else {
        QMessageBox::warning(this, "提示", ret["msg"].toString());
    }
}
