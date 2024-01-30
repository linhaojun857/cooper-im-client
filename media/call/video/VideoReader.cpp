#include "VideoReader.hpp"

#include <QTime>

VideoReader::VideoReader() = default;

VideoReader::~VideoReader() = default;

void VideoReader::run() {
    while (true) {
        if (m_isStop) {
            break;
        }
        slot_getVideoFrame();
        qint64 beginTime = QDateTime::currentMSecsSinceEpoch();
        while (QDateTime::currentMSecsSinceEpoch() - beginTime < 1000 / FRAME_RATE) {
            QThread::msleep(1);
        }
    }
}

void VideoReader::slot_getVideoFrame() {
    Mat frame;
    if (!m_cap.read(frame)) {
        return;
    }
    cvtColor(frame, frame, CV_BGR2RGB);
    QImage image((unsigned char*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
    double ratio = image.height() * 1.0 / image.width();
    int width = IMAGE_WIDTH;
    int height = (int)(ratio * width);
    image = image.scaled(width, height, Qt::KeepAspectRatio);
    Q_EMIT SIG_sendVideoFrame(image);
}

void VideoReader::slot_openVideo() {
    m_cap.open(0);
    if (!m_cap.isOpened()) {
        QMessageBox::information(nullptr, tr("提示"), tr("视频没有打开"));
        return;
    }
    m_isStop = false;
    this->start();
}

void VideoReader::slot_closeVideo() {
    m_isStop = true;
    if (m_cap.isOpened()) {
        m_cap.release();
    }
    this->wait();
}
