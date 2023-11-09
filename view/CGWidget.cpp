#include "CGWidget.hpp"

#include <QMessageBox>

#include "define/IMDefine.hpp"
#include "store/IMStore.hpp"
#include "ui_CGWidget.h"
#include "util/HttpUtil.hpp"

CGWidget::CGWidget(QWidget* parent) : QWidget(parent), ui(new Ui::CGWidget) {
    ui->setupUi(this);
    setWindowTitle("Cooper");
    setWindowIcon(QIcon(":/img/logo.ico"));
    connect(ui->m_createPushButton, &QPushButton::clicked, this, &CGWidget::handleClickCreatePushButton);
}

CGWidget::~CGWidget() {
    delete ui;
}

void CGWidget::handleClickCreatePushButton() {
    QString name = ui->m_nameLineEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "提示", "群名不能为空");
        return;
    }
    QString desc = ui->m_descTextEdit->toPlainText();
    QJsonObject json;
    json["token"] = IMStore::getInstance()->getToken();
    json["name"] = name;
    json["desc"] = desc;
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/group/createGroup", json);
    if (ret["code"].toInt() == HTTP_SUCCESS_CODE) {
        QMessageBox::information(this, "提示", "创建成功");
        close();
    } else {
        QMessageBox::warning(this, "提示", ret["msg"].toString());
    }
}
