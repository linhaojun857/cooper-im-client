#ifndef core_common_hpp
#define core_common_hpp
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libavutil/time.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
}
#include <QDebug>
#include <QMessageBox>
#include <QTime>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

using namespace cv;
#define STREAM_FRAME_RATE (25)
#define STREAM_PIX_FMT AV_PIX_FMT_YUV420P

#endif
