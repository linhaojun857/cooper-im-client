#include "RecordDialog.hpp"

#include "ui_RecordDialog.h"

RecordDialog::RecordDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RecordDialog) {
    ui->setupUi(this);
    setWindowFlag(Qt::WindowMinMaxButtonsHint);
    m_pictureWidget = new PictureWidget();
    m_pictureWidget->hide();
    m_pictureWidget->move(0, 0);
    m_saveVideoFileThread = new SaveVideoFileThread();
    connect(ui->pb_start, &QPushButton::clicked, m_saveVideoFileThread, [this]() {
        showMinimized();
        m_pictureWidget->show();
        STRU_AV_FORMAT format;
        format.fileName = m_saveUrl;
        format.frame_rate = STREAM_FRAME_RATE;
        format.hasCamera = true;
        format.hasDesk = true;
        format.hasAudio = true;
        format.videoBitRate = 1000000;
        format.width = 1920;
        format.height = 1080;
        m_saveVideoFileThread->slots_setInfo(format);
        m_saveVideoFileThread->slots_openVideo();
    });
    connect(ui->pb_end, &QPushButton::clicked, m_saveVideoFileThread, [this]() {
        m_pictureWidget->hide();
        m_saveVideoFileThread->slots_closeVideo();
        // 发一张黑色图片
        QImage image = QImage(1920, 1080, QImage::Format_RGB888);
        image.fill(Qt::black);
        slots_setImage(image);
    });
    connect(ui->pb_setUrl, &QPushButton::clicked, m_saveVideoFileThread, [this]() {
        m_saveUrl = ui->le_url->text();
        // m_saveUrl = m_saveUrl.replace("/", "\\");
    });
    connect(m_saveVideoFileThread, &SaveVideoFileThread::SIG_sendPicInPic, m_pictureWidget,
            &PictureWidget::slots_setImage);
    connect(m_saveVideoFileThread, &SaveVideoFileThread::SIG_sendVideoFrame, this, &RecordDialog::slots_setImage);
}

RecordDialog::~RecordDialog() {
    delete ui;
}

void RecordDialog::slots_setImage(const QImage& image) {
    if (image.isNull()) {
        return;
    }
    static QSize size(image.width(), ui->lb_showImage->height());
    QPixmap pixmap = QPixmap::fromImage(image.scaled(size, Qt::KeepAspectRatio));
    ui->lb_showImage->setPixmap(pixmap);
}
