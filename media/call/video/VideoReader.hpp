#ifndef media_call_video_VideoReader_hpp
#define media_call_video_VideoReader_hpp

#include <opencv2/imgproc/types_c.h>

#include <QImage>
#include <QMessageBox>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define IMAGE_WIDTH (320)
#define FRAME_RATE (20)

using namespace cv;

class VideoReader : public QThread {
    Q_OBJECT
public:
    explicit VideoReader();

    ~VideoReader() override;

    void run() override;

signals:
    void SIG_sendVideoFrame(const QImage& image);

public slots:
    void slot_getVideoFrame();
    void slot_openVideo();
    void slot_closeVideo();

private:
    VideoCapture m_cap;
    bool m_isStop = false;
};

#endif
