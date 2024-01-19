#include "LivePlayerDialog.hpp"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPoint>
#include <QScrollBar>
#include <QStyle>

#include "define/IMDefine.hpp"
#include "mock/Mock.hpp"
#include "ui_LivePlayerDialog.h"

LivePlayerDialog::LivePlayerDialog(QWidget* parent) : QDialog(parent), ui(new Ui::LivePlayerDialog) {
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/logo.ico"));

    m_player = new VideoPlayer();

    connect(ui->m_resumePB, SIGNAL(clicked()), this, SLOT(handleClickResumePB()));
    connect(ui->m_pausePB, SIGNAL(clicked()), this, SLOT(handleClickPausePB()));
    connect(ui->m_sendMsgPB, SIGNAL(clicked()), this, SLOT(handleClickSendMsgPB()));
    connect(m_player, SIGNAL(SIG_setOneImage(QImage)), this, SLOT(setImage(QImage)));
    connect(m_player, SIGNAL(SIG_PlayerStateChanged(int)), this, SLOT(slot_PlayerStateChanged(int)));
    connect(m_player, SIGNAL(SIG_TotalTime(qint64)), this, SLOT(slot_getTotalTime(qint64)));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slot_TimerTimeOut()));

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

void LivePlayerDialog::setImage(const QImage& image) {
    QPixmap pixmap;
    if (!image.isNull()) {
        pixmap = QPixmap::fromImage(image.scaled(ui->m_showLabel->size(), Qt::KeepAspectRatio));
    } else {
        pixmap = QPixmap::fromImage(image);
    }
    ui->m_showLabel->setPixmap(pixmap);
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
            setImage(img);
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
    // for test
    static int index = 0;
    auto liveRoomMsgItem = new LiveRoomMsgItem();
    liveRoomMsgItem->setAvatar(Mock::urls[index++ % Mock::urls.size()]);
    liveRoomMsgItem->setNicknameAndMsg(Mock::nicknames[index % Mock::nicknames.size()],
                                       QString("这是第%1条消息这是第%1条消息").arg(index));
    addLiveRoomMsgItem(liveRoomMsgItem);
    ui->m_scrollArea->verticalScrollBar()->setValue(ui->m_scrollArea->verticalScrollBar()->maximum());
    index++;
}
