#include "ApplyNotifyItem.hpp"

#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "define/IMDefine.hpp"
#include "store/IMStore.hpp"
#include "ui_ApplyNotifyItem.h"
#include "util/HttpUtil.hpp"

ApplyNotifyItem::ApplyNotifyItem(QWidget* parent) : QWidget(parent), ui(new Ui::ApplyNotifyItem) {
    ui->setupUi(this);
    connect(ui->m_agreePushButton, &QPushButton::clicked, std::bind(&ApplyNotifyItem::responseFriendApply, this, 1));
    connect(ui->m_rejectPushButton, &QPushButton::clicked, std::bind(&ApplyNotifyItem::responseFriendApply, this, 2));
}

ApplyNotifyItem::~ApplyNotifyItem() {
    delete ui;
}

void ApplyNotifyItem::setIPMode(int mode) {
    m_IPMode = mode;
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

void ApplyNotifyItem::setFGMode(int mode) {
    m_FGMode = mode;
    if (mode == 0) {
        ui->m_statusLabel->setText("等待验证");
    } else {
        ui->m_statusLabel->setText("等待审核");
    }
}

void ApplyNotifyItem::setFromId(int fromId) {
    m_fromId = fromId;
}

void ApplyNotifyItem::setAvatar(const QString& url) {
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

void ApplyNotifyItem::setName(const QString& nickname) {
    m_name = nickname;
    ui->m_nameLabel->setText(nickname);
}

void ApplyNotifyItem::setOperation(const QString& operation) {
    m_reason = operation;
    QFontMetrics fontMetrics(ui->m_operationLabel->font());
    QString elideText = fontMetrics.elidedText(operation, Qt::ElideRight, ui->m_operationLabel->width());
    ui->m_operationLabel->setText(elideText);
}

void ApplyNotifyItem::setStatus(const QString& status) {
    ui->m_statusLabel->setText(status);
}

void ApplyNotifyItem::setReason(const QString& reason) {
    ui->m_widget->setToolTip("验证信息：<br>" + reason);
}

void ApplyNotifyItem::responseFriendApply(int agree) {
    QJsonObject json;
    json["from_id"] = m_fromId;
    json["agree"] = agree;
    json["token"] = IMStore::getInstance()->getToken();
    if (m_FGMode == 0) {
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
    } else {
        auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/responseGroupApply", json);
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
}
