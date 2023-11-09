#include "AFDialog.hpp"

#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "define/IMDefine.hpp"
#include "store/IMStore.hpp"
#include "ui_AFDialog.h"
#include "util/HttpUtil.hpp"

AFDialog::AFDialog(QWidget* parent) : QWidget(parent), ui(new Ui::AFDialog) {
    ui->setupUi(this);
    connect(ui->m_completePushButton, &QPushButton::clicked, this, &AFDialog::addFriend);
}

AFDialog::~AFDialog() {
    delete ui;
}

void AFDialog::setId(int id) {
    m_id = id;
}

int AFDialog::getId() const {
    return m_id;
}

void AFDialog::setMode(int mode) {
    m_mode = mode;
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

void AFDialog::setName(const QString& name) {
    m_name = name;
    setWindowTitle(name);
}

QString AFDialog::getName() {
    return m_name;
}

void AFDialog::addFriend() {
    auto reason = ui->m_reasonTextEdit->toPlainText();
    if (reason.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写验证信息");
        return;
    }
    QJsonObject json;
    json["token"] = IMStore::getInstance()->getToken();
    json["reason"] = reason;
    if (m_mode == 0) {
        json["peerId"] = m_id;
        auto ret = HttpUtil::post(HTTP_SERVER_URL "/friend/addFriend", json);
        if (ret["code"] == HTTP_SUCCESS_CODE) {
            QMessageBox::information(this, "提示", "发送成功");
            close();
            if (!IMStore::getInstance()->haveFANItemI(m_id)) {
                auto fanItem = new ApplyNotifyItem();
                fanItem->setIPMode(0);
                fanItem->setFGMode(0);
                fanItem->setAvatar(m_avatarUrl);
                fanItem->setName(m_name);
                fanItem->setOperation("请求添加对方为好友");
                fanItem->setReason(reason);
                IMStore::getInstance()->getNotifyWidget()->addApplyNotifyItem(fanItem);
                IMStore::getInstance()->addFANItemI(m_id, fanItem);
            } else {
                auto fanItem = IMStore::getInstance()->getFANItemI(m_id);
                fanItem->setStatus("等待验证");
            }
        } else {
            QMessageBox::warning(this, "提示", ret["msg"].toString());
        }
    } else if (m_mode == 1) {
        json["group_id"] = m_id;
        auto ret = HttpUtil::post(HTTP_SERVER_URL "/group/addGroup", json);
        if (ret["code"] == HTTP_SUCCESS_CODE) {
            QMessageBox::information(this, "提示", "发送成功");
            close();
            if (!IMStore::getInstance()->haveFANItemI(m_id)) {
                auto ganItem = new ApplyNotifyItem();
                ganItem->setIPMode(0);
                ganItem->setFGMode(1);
                ganItem->setAvatar(m_avatarUrl);
                ganItem->setName(m_name);
                ganItem->setOperation("申请加入群聊");
                ganItem->setReason(reason);
                IMStore::getInstance()->getNotifyWidget()->addApplyNotifyItem(ganItem);
                IMStore::getInstance()->addGANItemI(m_id, ganItem);
            } else {
                auto fanItem = IMStore::getInstance()->getFANItemI(m_id);
                fanItem->setStatus("等待审核");
            }
        } else {
            QMessageBox::warning(this, "提示", ret["msg"].toString());
        }
    }
}
