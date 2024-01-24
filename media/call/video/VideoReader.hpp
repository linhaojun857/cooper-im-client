#ifndef media_call_video_VideoReader_hpp
#define media_call_video_VideoReader_hpp

#include <opencv2/imgproc/types_c.h>

#include <QImage>
#include <QMessageBox>
#include <QObject>
#include <QTimer>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define IMAGE_WIDTH (320)
#define IMAGE_HEIGHT (240)
#define FRAME_RATE (10)

using namespace cv;

class VideoReader : public QObject {
    Q_OBJECT
public:
    explicit VideoReader(QObject* parent = nullptr);

    ~VideoReader() override;

signals:
    void SIG_sendVideoFrame(QImage img);

public slots:
    void slot_getVideoFrame();
    void slot_openVideo();
    void slot_closeVideo();

private:
    QTimer* m_timer;
    VideoCapture m_cap;
};

#endif
