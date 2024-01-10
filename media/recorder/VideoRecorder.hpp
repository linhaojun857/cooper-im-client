#ifndef core_VideoRecorder_hpp
#define core_VideoRecorder_hpp

#include <QApplication>
#include <QBuffer>
#include <QImage>
#include <QImageReader>
#include <QObject>
#include <QPainter>
#include <QScreen>
#include <QThread>
#include <QTime>
#include <QTimer>

#include "media/recorder/common.hpp"

class VideoRecorder : public QThread {
    Q_OBJECT
public:
    explicit VideoRecorder(QObject* parent = nullptr);

    void run() override;

signals:
    void SIG_sendVideoFrame(QImage img);                                 // 用于预览
    void SIG_sendVideoFrameData(uint8_t* picture_buf, int buffer_size);  // 采集的数据格式 YUV420P
    void SIG_sendPicInPic(QImage img);                                   // 用于显示画中画
public slots:
    void slot_getVideoFrame();  // 定时器周期获取画面
    void slot_openVideo();      // 开启采集
    void slot_closeVideo();     // 关闭采集

private:
    VideoCapture cap;                                       // opencv 采集摄像头对象
    QTimer* timer = nullptr;                                // 定时器
    bool isStop = false;                                    // 是否停止采集
    int ImageToYuvBuffer(QImage& image, uint8_t** buffer);  // RGB24 转为 yuv420p
};

#endif
