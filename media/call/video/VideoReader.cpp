#include "VideoReader.hpp"

VideoReader::VideoReader(QObject* parent) : QObject(parent) {
    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_getVideoFrame()));
}

VideoReader::~VideoReader() {
    if (m_timer) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
}

void VideoReader::slot_getVideoFrame() {
    Mat frame;
    if (!m_cap.read(frame)) {
        return;
    }
    cvtColor(frame, frame, CV_BGR2RGB);
    QImage image((unsigned const char*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
    image = image.scaled(IMAGE_WIDTH, IMAGE_HEIGHT, Qt::KeepAspectRatio);
    Q_EMIT SIG_sendVideoFrame(image);
}

void VideoReader::slot_openVideo() {
    m_timer->start(1000 / FRAME_RATE - 10);
    m_cap.open(0);
    if (!m_cap.isOpened()) {
        QMessageBox::information(nullptr, tr("提示"), tr("视频没有打开"));
        return;
    }
}

void VideoReader::slot_closeVideo() {
    m_timer->stop();
    if (m_cap.isOpened())
        m_cap.release();
}
