#include "OpenLiveWidget.hpp"

#include <QFileDialog>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "define/IMDefine.hpp"
#include "store/IMStore.hpp"
#include "ui_OpenLiveWidget.h"
#include "util/HttpUtil.hpp"

OpenLiveWidget::OpenLiveWidget(QWidget* parent) : QWidget(parent), ui(new Ui::OpenLiveWidget) {
    ui->setupUi(this);
    setWindowTitle("开启直播");
    setWindowIcon(QIcon(":/img/logo.ico"));
    ui->m_liveRoomCoverLabel->installEventFilter(this);
    connect(this, &OpenLiveWidget::liveRoomCoverClicked, this, &OpenLiveWidget::handleLiveRoomCoverClicked);
    connect(ui->m_openLivePushButton, &QPushButton::clicked, this, &OpenLiveWidget::handleOpenLivePushButtonClicked);
}

OpenLiveWidget::~OpenLiveWidget() {
    delete ui;
}

bool OpenLiveWidget::eventFilter(QObject* watched, QEvent* event) {
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

void OpenLiveWidget::reset() {
    ui->m_liveRoomCoverLabel->clear();
    ui->m_liveRoomCoverLabel->setText("点击上传直播封面");
}

void OpenLiveWidget::handleLiveRoomCoverClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "选择直播封面", "E:/", "Images (*.png *.jpg *.jpeg)");
    if (fileName.isEmpty()) {
        return;
    }
    std::ignore = QtConcurrent::run([=]() {
        QString retUrl = HttpUtil::upload(fileName);
        m_liveRoomCoverUrl = retUrl;
        ui->m_liveRoomCoverLabel->clear();
        ui->m_liveRoomCoverLabel->setPixmap(QPixmap(fileName));
    });
}

void OpenLiveWidget::handleOpenLivePushButtonClicked() {
    QJsonObject json;
    json["cover"] = m_liveRoomCoverUrl;
    json["token"] = IMStore::getInstance()->getToken();
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/live/openLive", json);
    if (ret["code"].toInt() == HTTP_SUCCESS_CODE) {
        qDebug() << ret["msg"].toString();
        int roomId = ret["room_id"].toInt();
        auto liveRecordDialog = IMStore::getInstance()->getLiveRecordDialog();
        liveRecordDialog->show();
        liveRecordDialog->start(roomId);
    } else {
        QMessageBox::warning(this, "提示", ret["msg"].toString());
    }
    this->close();
}
