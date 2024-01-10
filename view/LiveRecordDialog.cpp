#include "LiveRecordDialog.hpp"

#include "define/IMDefine.hpp"
#include "ui_LiveRecordDialog.h"

LiveRecordDialog::LiveRecordDialog(QWidget* parent) : QDialog(parent), ui(new Ui::LiveRecordDialog) {
    ui->setupUi(this);
    setWindowFlag(Qt::WindowMinMaxButtonsHint);
    m_pictureWidget = new LivePictureWidget();
    m_pictureWidget->hide();
    m_pictureWidget->move(0, 0);
    m_saveVideoFileThread = new SaveVideoFileThread();
    connect(ui->pb_pause, &QPushButton::clicked, this, &LiveRecordDialog::handleClickPbPause);
    connect(ui->pb_end, &QPushButton::clicked, this, &LiveRecordDialog::handleClickPbEnd);
    connect(m_saveVideoFileThread, &SaveVideoFileThread::SIG_sendPicInPic, m_pictureWidget,
            &LivePictureWidget::slots_setImage);
    connect(m_saveVideoFileThread, &SaveVideoFileThread::SIG_sendVideoFrame, this, &LiveRecordDialog::slots_setImage);
}

LiveRecordDialog::~LiveRecordDialog() {
    delete ui;
}

void LiveRecordDialog::start(int roomId) {
    showMinimized();
    m_pictureWidget->show();
    STRU_AV_FORMAT format;
    format.fileName = LIVE_BASE_URL + QString::number(roomId);
    format.frame_rate = STREAM_FRAME_RATE;
    format.hasCamera = true;
    format.hasDesk = true;
    format.hasAudio = true;
    format.videoBitRate = 1000000;
    format.width = 1920;
    format.height = 1080;
    m_saveVideoFileThread->slots_setInfo(format);
    m_saveVideoFileThread->slots_openVideo();
}

void LiveRecordDialog::slots_setImage(const QImage& image) {
    if (image.isNull()) {
        return;
    }
    static QSize size(image.width(), ui->lb_showImage->height());
    QPixmap pixmap = QPixmap::fromImage(image.scaled(size, Qt::KeepAspectRatio));
    ui->lb_showImage->setPixmap(pixmap);
}

void LiveRecordDialog::handleClickPbPause() {
}

void LiveRecordDialog::handleClickPbEnd() {
    m_pictureWidget->hide();
    m_saveVideoFileThread->slots_closeVideo();
    QImage image = QImage(1920, 1080, QImage::Format_RGB888);
    image.fill(Qt::black);
    slots_setImage(image);
}
