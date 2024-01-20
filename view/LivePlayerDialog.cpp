#include "LivePlayerDialog.hpp"

#include <QInputDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPoint>
#include <QScrollBar>
#include <QStyle>
#include <QtConcurrent/QtConcurrent>

#include "core/IMKernel.hpp"
#include "define/IMDefine.hpp"
#include "mock/Mock.hpp"
#include "store/IMStore.hpp"
#include "ui_LivePlayerDialog.h"
#include "util/HttpUtil.hpp"

LivePlayerDialog::LivePlayerDialog(QWidget* parent) : QDialog(parent), ui(new Ui::LivePlayerDialog) {
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/logo.ico"));
    setWindowTitle("直播间");

    m_player = new VideoPlayer();

    connect(ui->m_resumePB, SIGNAL(clicked()), this, SLOT(handleClickResumePB()));
    connect(ui->m_pausePB, SIGNAL(clicked()), this, SLOT(handleClickPausePB()));
    connect(ui->m_sendMsgPB, SIGNAL(clicked()), this, SLOT(handleClickSendMsgPB()));
    connect(m_player, SIGNAL(SIG_setOneImage(QImage)), this, SLOT(slot_setImage(QImage)));
    connect(m_player, SIGNAL(SIG_PlayerStateChanged(int)), this, SLOT(slot_PlayerStateChanged(int)));
    connect(m_player, SIGNAL(SIG_TotalTime(qint64)), this, SLOT(slot_getTotalTime(qint64)));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slot_TimerTimeOut()));

    connect(this, &LivePlayerDialog::finished, [=](int result) {
        (void)result;
        slot_PlayerStateChanged(PlayerState::Stop);
        m_player->stop(true);
        QJsonObject json;
        json["token"] = IMStore::getInstance()->getToken();
        json["room_id"] = m_liveRoomId;
        auto ret = HttpUtil::post(HTTP_SERVER_URL "/live/leaveLive", json);
        if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
            return;
        }
    });

    m_player->m_playerState = PlayerState::Stop;
    slot_PlayerStateChanged(PlayerState::Stop);
    ui->m_progressSlider->installEventFilter(this);
    m_timer.setInterval(500);

    ui->m_scrollArea->setFrameStyle(QFrame::NoFrame);
    ui->m_scrollArea->setFrameShape(QFrame::NoFrame);
    ui->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto widget = new QWidget();
    ui->m_scrollArea->setWidget(widget);

    m_msgLayout = new QVBoxLayout();
    m_msgLayout->setContentsMargins(0, 0, 0, 0);
    m_msgLayout->setSpacing(0);
    widget->setLayout(m_msgLayout);
    m_msgLayout->addStretch();
}

LivePlayerDialog::~LivePlayerDialog() {
    delete ui;
}

void LivePlayerDialog::start(int roomId) {
    QString url = LIVE_BASE_URL + QString::number(roomId);
    m_player->setFileName(url);
    slot_PlayerStateChanged(PlayerState::Playing);
}

void LivePlayerDialog::addLiveRoomMsgItem(LiveRoomMsgItem* liveRoomMsgItem) {
    m_msgLayout->insertWidget(m_msgLayout->count() - 1, liveRoomMsgItem);
}

void LivePlayerDialog::setLiveRoomId(int roomId) {
    m_liveRoomId = roomId;
    QJsonObject json;
    json["room_id"] = roomId;
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/live/getOpenedLiveInfoByRoomId", json);
    if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
        return;
    }
    auto liveRoom = ret["live_room"].toObject();
    setLiveRoomOwnerAvatar(liveRoom["owner_avatar"].toString());
    setLiveRoomOwnerName(liveRoom["owner_nickname"].toString());
    setLiveRoomViewerCount(liveRoom["viewer_count"].toInt());
}

void LivePlayerDialog::closeLive() {
    slot_PlayerStateChanged(PlayerState::Stop);
    m_player->stop(true);
}

void LivePlayerDialog::setLiveRoomOwnerAvatar(const QString& liveRoomOwnerAvatarUrl) {
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

void LivePlayerDialog::setLiveRoomOwnerName(const QString& liveRoomOwnerName) {
    ui->m_liveRoomOwnerNameLabel->setText(liveRoomOwnerName);
}

void LivePlayerDialog::setLiveRoomViewerCount(int liveRoomViewerCount) {
    ui->m_liveRoomViewerCountLabel->setText("观看人数: " + QString::number(liveRoomViewerCount));
}

void LivePlayerDialog::handleClickResumePB() {
    if (m_player->getPlayerState() != Pause)
        return;
    m_player->play();
}

void LivePlayerDialog::handleClickPausePB() {
    if (m_player->getPlayerState() != Playing)
        return;
    m_player->pause();
}

void LivePlayerDialog::slot_setImage(const QImage& image) {
    if (image.isNull()) {
        return;
    }
    if (m_adjustFlag) {
        double ratio = image.height() * 1.0 / image.width();
        double height = ratio * ui->m_showLabel->width();
        if (height > ui->m_showLabel->height()) {
            height = ui->m_showLabel->height();
        }
        ui->m_showLabel->setFixedSize(ui->m_showLabel->width(), (int)height);
        ui->m_showLabel->move((ui->m_showLabel->parentWidget()->width() - ui->m_showLabel->width()) / 2,
                              (ui->m_showLabel->parentWidget()->height() - ui->m_showLabel->height()) / 2);
        m_adjustFlag = false;
    }
    ui->m_showLabel->setPixmap(QPixmap::fromImage(image));
}

void LivePlayerDialog::slot_PlayerStateChanged(int state) {
    switch (state) {
        case PlayerState::Stop: {
            qDebug() << "VideoPlayer::Stop";
            m_timer.stop();
            ui->m_progressSlider->setValue(0);
            ui->m_totalTimeLabel->setText("00:00:00");
            ui->m_curtTimeLabel->setText("00:00:00");
            QImage img;
            img.fill(Qt::black);
            slot_setImage(img);
            this->update();
            break;
        }
        case PlayerState::Playing: {
            qDebug() << "VideoPlayer::Playing";
            m_timer.start();
            this->update();
            break;
        }
        default:
            break;
    }
}

void LivePlayerDialog::slot_getTotalTime(qint64 uSec) {
    qint64 Sec = uSec / 1000000;
    ui->m_progressSlider->setRange(0, (int)Sec);
    QString hStr = QString("00%1").arg(Sec / 3600);
    QString mStr = QString("00%1").arg(Sec / 60);
    QString sStr = QString("00%1").arg(Sec % 60);
    QString str = QString("%1:%2:%3").arg(hStr.right(2)).arg(mStr.right(2)).arg(sStr.right(2));
    ui->m_totalTimeLabel->setText(str);
}

void LivePlayerDialog::slot_TimerTimeOut() {
    if (QObject::sender() == &m_timer) {
        qint64 Sec = (qint64)m_player->getCurrentTime() / 1000000;
        ui->m_progressSlider->setValue((int)Sec);
        QString hStr = QString("00%1").arg(Sec / 3600);
        QString mStr = QString("00%1").arg(Sec / 60 % 60);
        QString sStr = QString("00%1").arg(Sec % 60);
        QString str = QString("%1:%2:%3").arg(hStr.right(2)).arg(mStr.right(2)).arg(sStr.right(2));
        ui->m_curtTimeLabel->setText(str);
        if ((ui->m_progressSlider->value() == ui->m_progressSlider->maximum() &&
             m_player->m_playerState == PlayerState::Stop) ||
            (ui->m_progressSlider->value() + 1 == ui->m_progressSlider->maximum() &&
             m_player->m_playerState == PlayerState::Stop)) {
            slot_PlayerStateChanged(PlayerState::Stop);
        }
    }
}

bool LivePlayerDialog::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->m_progressSlider) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
            int min = ui->m_progressSlider->minimum();
            int max = ui->m_progressSlider->maximum();
            int value = QStyle::sliderValueFromPosition(min, max, mouseEvent->pos().x(), ui->m_progressSlider->width());
            m_timer.stop();
            ui->m_progressSlider->setValue(value);
            m_player->seek(value * 1000000);
            m_timer.start();
            return true;
        } else {
            return false;
        }
    }
    return QDialog::eventFilter(watched, event);
}

void LivePlayerDialog::handleClickSendMsgPB() {
    QJsonObject json;
    json["type"] = PROTOCOL_TYPE_LIVE_ROOM_MSG_SEND;
    json["token"] = IMStore::getInstance()->getToken();
    json["room_id"] = m_liveRoomId;
    QString msg = ui->m_plainTextEdit->toPlainText();
    json["msg"] = msg;
    ui->m_plainTextEdit->clear();
    auto item = new LiveRoomMsgItem();
    item->setAvatar(IMStore::getInstance()->getSelf()->avatar);
    item->setNicknameAndMsg(IMStore::getInstance()->getSelf()->nickname, msg);
    addLiveRoomMsgItem(item);
    IMStore::getInstance()->getIMKernel()->sendLiveMsg(json);
}
