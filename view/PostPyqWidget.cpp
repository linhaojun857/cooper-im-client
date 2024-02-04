#include "PostPyqWidget.hpp"

#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "define/IMDefine.hpp"
#include "store/IMStore.hpp"
#include "ui_PostPyqWidget.h"
#include "util/HttpUtil.hpp"

PostPyqWidget::PostPyqWidget(QWidget* parent) : QWidget(parent), ui(new Ui::PostPyqWidget) {
    ui->setupUi(this);
    setWindowTitle("发布朋友圈");
    setWindowIcon(QIcon(":/img/logo.ico"));

    m_imageLayout = new QGridLayout();
    m_imageLayout->setContentsMargins(0, 0, 0, 0);
    m_imageLayout->setSpacing(0);
    m_imageLayout->setAlignment(Qt::AlignTop);
    m_imageLayout->setColumnMinimumWidth(0, 130);
    m_imageLayout->setColumnMinimumWidth(1, 130);
    m_imageLayout->setColumnMinimumWidth(2, 130);
    ui->m_imageWidget->setLayout(m_imageLayout);

    m_uploadPushButton = new QPushButton();
    m_uploadPushButton->setFixedSize(130, 130);
    m_uploadPushButton->setFont(QFont("Microsoft YaHei UI", 20));
    m_uploadPushButton->setText("+");
    m_uploadPushButton->setStyleSheet("border: 1px solid #000000;");
    m_imageLayout->addWidget(m_uploadPushButton, 0, 0);

    connect(ui->m_publishPushButton, &QPushButton::clicked, this, &PostPyqWidget::handleClickPublishPushButton);
    connect(m_uploadPushButton, &QPushButton::clicked, this, &PostPyqWidget::handleClickUploadPushButton);
}

PostPyqWidget::~PostPyqWidget() {
    delete ui;
    delete m_imageLayout;
    for (auto label : m_imageLabels) {
        delete label;
    }
    delete m_uploadPushButton;
}

void PostPyqWidget::handleClickPublishPushButton() {
    QJsonObject json;
    json["token"] = IMStore::getInstance()->getToken();
    json["content"] = ui->m_contentTextEdit->toPlainText();
    QJsonArray imageUrls;
    for (const auto& url : m_imageUrls) {
        imageUrls.append(url);
    }
    json["image_urls"] = QJsonDocument(imageUrls).toJson(QJsonDocument::Compact).data();
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/pyq/postPyq", json);
    if (ret["code"].toInt() == HTTP_SUCCESS_CODE) {
        close();
    } else {
        qDebug() << "发布失败: " << ret["msg"].toString();
    }
}

void PostPyqWidget::handleClickUploadPushButton() {
    if (m_imageUrls.size() == 9) {
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this, "选择直播封面", "E:/", "Images (*.png *.jpg *.jpeg)");
    if (fileName.isEmpty()) {
        return;
    }
    auto label = new QLabel();
    label->setFixedSize(130, 130);
    std::ignore = QtConcurrent::run([=]() {
        QString retUrl = HttpUtil::upload(fileName);
        m_imageUrls.append(retUrl);
        auto manager = new QNetworkAccessManager();
        QNetworkRequest request(retUrl);
        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data);
            pixmap = pixmap.scaled(label->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            label->setPixmap(pixmap);
        } else {
            qDebug() << "load failed: " << reply->errorString();
        }
        if (m_imageUrls.size() == 3 || m_imageUrls.size() == 6) {
            changeLayoutWhenAddImage(130);
        }
        m_imageLayout->addWidget(label, (int)((m_imageUrls.size() - 1) / 3), (int)((m_imageUrls.size() - 1) % 3));
        if (m_imageUrls.size() != 9) {
            m_imageLayout->addWidget(m_uploadPushButton, (int)(m_imageUrls.size() / 3), (int)(m_imageUrls.size() % 3));
        }
        m_imageLabels.append(label);
    });
}

void PostPyqWidget::changeLayoutWhenAddImage(int deltaHeight) {
    ui->m_imageWidget->setFixedHeight(ui->m_imageWidget->height() + deltaHeight);
    setMinimumSize(width(), height() + deltaHeight);
    setMaximumSize(width(), height() + deltaHeight);
    ui->m_publishPushButton->move(ui->m_publishPushButton->x(), ui->m_publishPushButton->y() + deltaHeight);
}
