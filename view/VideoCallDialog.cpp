#include "VideoCallDialog.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "store/IMStore.hpp"
#include "ui_VideoCallDialog.h"

VideoCallDialog::VideoCallDialog(QWidget* parent) : QWidget(parent), ui(new Ui::VideoCallDialog) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    IMStore::getInstance()->setVideoCallDialog(this);

    m_audioReader = new AudioReader();
    m_audioWriter = new AudioWriter();
    m_videoReader = new VideoReader();
    connect(m_audioReader, &AudioReader::SIG_audioFrame, this, &VideoCallDialog::handleAudioFrame,
            Qt::DirectConnection);
    connect(m_videoReader, &VideoReader::SIG_sendVideoFrame, this, &VideoCallDialog::handleVideoFrame,
            Qt::DirectConnection);
}

VideoCallDialog::~VideoCallDialog() {
    delete ui;
    delete m_audioReader;
    delete m_audioWriter;
    delete m_videoReader;
    IMStore::getInstance()->setVideoCallDialog(nullptr);
}

void VideoCallDialog::setPeerId(int peerId) {
    m_peerId = peerId;
    QString peerAvatar = IMStore::getInstance()->getFriend(peerId)->avatar;
    QString peerNickname = IMStore::getInstance()->getFriend(peerId)->nickname;
    setAvatar(peerAvatar);
    setWindowTitle(peerNickname);
}

void VideoCallDialog::setAvatar(const QString& url) {
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

void VideoCallDialog::showMessageBox(const QString& content) {
    QMessageBox box;
    box.setWindowIcon(QIcon(":/img/logo.ico"));
    box.setWindowTitle("提示");
    box.setText(content);
    box.setIcon(QMessageBox::Information);
    box.setStandardButtons(QMessageBox::Ok);
    box.exec();
    close();
}

void VideoCallDialog::startCall() {
    IMStore::getInstance()->getIMKernel()->openAVCall();
    m_mediator = IMStore::getInstance()->getIMKernel()->getAVCallMediator();
    m_audioReader->openAudio();
    m_videoReader->slot_openVideo();
}

void VideoCallDialog::closeCall() {
    m_audioReader->pauseAudio();
    m_videoReader->slot_closeVideo();
    IMStore::getInstance()->getIMKernel()->closeAVCall();
}

void VideoCallDialog::playAudio(const QByteArray& arr) {
    m_audioWriter->slot_netRx(arr);
}

void VideoCallDialog::playVideo(const QImage& image) {
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

void VideoCallDialog::handleAudioFrame(const QByteArray& arr) {
    int size = (int)(4 + 4 + 4 + 8 + arr.size());
    int type = PROTOCOL_TYPE_VIDEO_CALL_AUDIO_FRAME;
    int fromId = IMStore::getInstance()->getSelf()->id;
    int toId = m_peerId;
    long long timestamp = QDateTime::currentMSecsSinceEpoch();
    if (m_mediator) {
        m_mutex.lock();
        m_mediator->sendRaw((char*)&size, sizeof(size));
        m_mediator->sendRaw((char*)&type, sizeof(type));
        m_mediator->sendRaw((char*)&fromId, sizeof(fromId));
        m_mediator->sendRaw((char*)&toId, sizeof(toId));
        m_mediator->sendRaw((char*)&timestamp, sizeof(timestamp));
        m_mediator->sendRaw((char*)arr.data(), (int)arr.size());
        m_mutex.unlock();
    }
}

void VideoCallDialog::handleVideoFrame(const QImage& image) {
    int size = (int)(4 + 4 + 4 + 8 + 4 + 4 + image.sizeInBytes());
    int type = PROTOCOL_TYPE_VIDEO_CALL_VIDEO_FRAME;
    int fromId = IMStore::getInstance()->getSelf()->id;
    int toId = m_peerId;
    long long timestamp = QDateTime::currentMSecsSinceEpoch();
    int width = image.width();
    int height = image.height();
    if (m_mediator) {
        m_mutex.lock();
        m_mediator->sendRaw((char*)&size, sizeof(size));
        m_mediator->sendRaw((char*)&type, sizeof(type));
        m_mediator->sendRaw((char*)&fromId, sizeof(fromId));
        m_mediator->sendRaw((char*)&toId, sizeof(toId));
        m_mediator->sendRaw((char*)&timestamp, sizeof(timestamp));
        m_mediator->sendRaw((char*)&width, sizeof(width));
        m_mediator->sendRaw((char*)&height, sizeof(height));
        m_mediator->sendRaw((char*)image.bits(), (int)image.sizeInBytes());
        m_mutex.unlock();
    }
}

void VideoCallDialog::closeEvent(QCloseEvent* event) {
    closeCall();
    QWidget::closeEvent(event);
}
