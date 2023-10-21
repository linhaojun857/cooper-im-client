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
    ui->m_reasonTextEdit->setPlaceholderText("我是...");
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

void AFDialog::addFriend() {
    auto reason = ui->m_reasonTextEdit->toPlainText();
    if (reason.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写验证信息");
        return;
    }
    QJsonObject json;
    json["reason"] = reason;
    json["peerId"] = m_id;
    json["token"] = IMStore::getInstance()->getToken();
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/addFriend", json);
    if (ret["code"] == HTTP_SUCCESS_CODE) {
        QMessageBox::information(this, "提示", "发送成功");
        close();
    } else {
        QMessageBox::warning(this, "提示", ret["msg"].toString());
    }
}
