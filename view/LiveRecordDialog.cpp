#include "LiveRecordDialog.hpp"

#include "define/IMDefine.hpp"
#include "ui_LiveRecordDialog.h"

LiveRecordDialog::LiveRecordDialog(QWidget* parent) : QDialog(parent), ui(new Ui::LiveRecordDialog) {
    ui->setupUi(this);

    setWindowIcon(QIcon(":/img/logo.ico"));

    setWindowFlag(Qt::WindowMinMaxButtonsHint);
    m_pictureWidget = new LivePictureWidget();
    m_pictureWidget->hide();
    m_pictureWidget->move(0, 0);
    m_saveVideoFileThread = new SaveVideoFileThread();
    connect(this, &LiveRecordDialog::finished, this, [=](int result) {
        (void)result;
        handleClickPbEnd();
    });
    connect(ui->pb_pause, &QPushButton::clicked, this, &LiveRecordDialog::handleClickPbPause);
    connect(ui->pb_end, &QPushButton::clicked, this, &LiveRecordDialog::handleClickPbEnd);
    connect(ui->cb_camera, &QCheckBox::stateChanged, [=](int state) {
        if (state == Qt::Checked) {
            m_pictureWidget->show();
        } else {
            m_pictureWidget->hide();
        }
    });
    connect(m_saveVideoFileThread, &SaveVideoFileThread::SIG_sendPicInPic, m_pictureWidget,
            &LivePictureWidget::slots_setImage);
    connect(m_saveVideoFileThread, &SaveVideoFileThread::SIG_sendVideoFrame, this, &LiveRecordDialog::slots_setImage);
}

LiveRecordDialog::~LiveRecordDialog() {
    delete ui;
}

void LiveRecordDialog::start(int roomId) {
    showMinimized();
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
    if (m_adjustFlag) {
        double ratio = image.height() * 1.0 / image.width();
        double height = ratio * ui->lb_showImage->width();
        if (height > ui->lb_showImage->height()) {
            height = ui->lb_showImage->height();
        }
        ui->lb_showImage->setFixedSize(ui->lb_showImage->width(), (int)height);
        ui->lb_showImage->move((ui->lb_showImage->parentWidget()->width() - ui->lb_showImage->width()) / 2,
                               (ui->lb_showImage->parentWidget()->height() - ui->lb_showImage->height()) / 2);
        m_adjustFlag = false;
    }
    ui->lb_showImage->setPixmap(QPixmap::fromImage(image));
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
