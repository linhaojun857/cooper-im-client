#include "LRWidget.hpp"

#include <QMessageBox>
#include <QRegExp>
#include <QTabBar>
#include <QTimer>

#include "config/IMDefine.hpp"
#include "ui_LRWidget.h"
#include "util/HttpUtil.hpp"

LRWidget::LRWidget(QWidget* parent) : QWidget(parent), ui(new Ui::LRWidget) {
    ui->setupUi(this);
    this->setWindowTitle("Cooper");
    ui->m_lRTabWidget->tabBar()->hide();
    connect(ui->m_registerPushButton, &QPushButton::clicked, this, [&]() {
        ui->m_lRTabWidget->setCurrentIndex(1);
    });
    connect(ui->m_loginPushButton_r, &QPushButton::clicked, this, [&]() {
        ui->m_lRTabWidget->setCurrentIndex(0);
    });
    connect(ui->m_getVfCodepushButton, &QPushButton::clicked, this, [&]() {
        getVFCode(ui->m_getVfCodepushButton);
    });
    connect(ui->m_getVfCodepushButton_r, &QPushButton::clicked, this, [&]() {
        getVFCode(ui->m_getVfCodepushButton_r);
    });
    connect(ui->m_loginPushButton, &QPushButton::clicked, this, &LRWidget::userLogin);
    connect(ui->m_registerPushButton_r, &QPushButton::clicked, this, &LRWidget::userRegister);
}

LRWidget::~LRWidget() {
    delete ui;
}

void LRWidget::getVFCode(QPushButton* pushButton) {
    QString username = ui->m_phoneLineEdit->text();
    QString usernameTemp = username;
    if (username.isEmpty() || usernameTemp.remove(" ").isEmpty()) {
        QMessageBox::about(this, "提示", "请输入手机号码");
        return;
    }
    pushButton->setEnabled(false);
    pushButton->setText("60s");
    int* count = new int(60);
    auto* timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, [=]() {
        (*count)--;
        pushButton->setText(QString("%1s").arg(*count));
        if (*count == 0) {
            pushButton->setEnabled(true);
            pushButton->setText("Get");
            timer->stop();
            delete count;
            delete timer;
        }
    });
    QJsonObject json;
    json["type"] = GET_VFCODE_TYPE;
    json["username"] = username;
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/getVFCode", json);
    if (ret["code"].toInt() == 20000) {
        QMessageBox::about(this, "提示", "验证码已发送");
    } else {
        QMessageBox::about(this, "提示", ret["msg"].toString());
    }
}

void LRWidget::userLogin() {
    QString username = ui->m_phoneLineEdit->text();
    QString usernameTemp = username;
    if (username.isEmpty() || usernameTemp.remove(" ").isEmpty()) {
        QMessageBox::about(this, "提示", "请输入手机号码");
        return;
    }
    QString password = ui->m_passwordLineEdit->text();
    QString passwordTemp = password;
    if (password.isEmpty() || passwordTemp.remove(" ").isEmpty()) {
        QMessageBox::about(this, "提示", "请输入密码");
        return;
    }
    QString vfCode = ui->m_vfCodeLineEdit->text();
    QString vfCodeTemp = vfCode;
    if (vfCode.isEmpty() || vfCodeTemp.remove(" ").isEmpty()) {
        QMessageBox::about(this, "提示", "请输入验证码");
        return;
    }
    QRegExp rx(R"(^1(3\d|4[5-9]|5[0-35-9]|6[2567]|7[0-8]|8\d|9[0-35-9])\d{8}$)");
    if (!rx.exactMatch(username)) {
        QMessageBox::about(this, "提示", "请输入正确的手机号码");
        return;
    }
    if (password.length() > 20) {
        QMessageBox::about(this, "提示", "密码长度不超过20");
        return;
    }
    QJsonObject json;
    json["type"] = LOGIN_TYPE;
    json["username"] = username;
    json["password"] = password;
    json["vfCode"] = vfCode;
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/login", json);
    if (ret["code"].toInt() == 20000) {
        QMessageBox::about(this, "提示", "登录成功");
        resetLoginWidget();
        // TODO: 登录成功的后续逻辑
    } else {
        QMessageBox::about(this, "提示", ret["msg"].toString());
    }
}

void LRWidget::userRegister() {
    QString username = ui->m_phoneLineEdit_r->text();
    QString usernameTemp = username;
    if (username.isEmpty() || usernameTemp.remove(" ").isEmpty()) {
        QMessageBox::about(this, "提示", "请输入手机号码");
        return;
    }
    QString password = ui->m_passwordLineEdit_r->text();
    QString passwordTemp = password;
    if (password.isEmpty() || passwordTemp.remove(" ").isEmpty()) {
        QMessageBox::about(this, "提示", "请输入密码");
        return;
    }
    QString rePassword = ui->m_rePasswordLineEdit_r->text();
    QString rePasswordTemp = rePassword;
    if (rePassword.isEmpty() || rePasswordTemp.remove(" ").isEmpty()) {
        QMessageBox::about(this, "提示", "请再次输入密码");
        return;
    }
    QString vfCode = ui->m_vfCodeLineEdit_r->text();
    QString vfCodeTemp = vfCode;
    if (vfCode.isEmpty() || vfCodeTemp.remove(" ").isEmpty()) {
        QMessageBox::about(this, "提示", "请输入验证码");
        return;
    }
    QRegExp rx(R"(^1(3\d|4[5-9]|5[0-35-9]|6[2567]|7[0-8]|8\d|9[0-35-9])\d{8}$)");
    if (!rx.exactMatch(username)) {
        QMessageBox::about(this, "提示", "请输入正确的手机号码");
        return;
    }
    if (password.length() > 20) {
        QMessageBox::about(this, "提示", "密码长度不超过20");
        return;
    }
    if (password != rePassword) {
        QMessageBox::about(this, "提示", "两次输入的密码不一致");
        return;
    }
    QJsonObject json;
    json["type"] = REGISTER_TYPE;
    json["username"] = username;
    json["password"] = password;
    json["vfCode"] = vfCode;
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/register", json);
    if (ret["code"].toInt() == 20000) {
        QMessageBox::about(this, "提示", "注册成功");
        resetRegisterWidget();
    } else {
        QMessageBox::about(this, "提示", ret["msg"].toString());
    }
}

void LRWidget::resetLoginWidget() {
    ui->m_phoneLineEdit->clear();
    ui->m_passwordLineEdit->clear();
    ui->m_vfCodeLineEdit->clear();
}

void LRWidget::resetRegisterWidget() {
    ui->m_phoneLineEdit_r->clear();
    ui->m_passwordLineEdit_r->clear();
    ui->m_rePasswordLineEdit_r->clear();
    ui->m_vfCodeLineEdit_r->clear();
}
