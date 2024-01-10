#ifndef core_SaveVideoFileThread_hpp
#define core_SaveVideoFileThread_hpp

#include <QMutex>
#include <QThread>

#include "media/recorder/AudioRecorder.hpp"
#include "media/recorder/VideoRecorder.hpp"

struct STRU_AV_FORMAT {
    void clear() {
        hasCamera = false;
        hasDesk = false;
        hasAudio = false;
        width = 0;
        height = 0;
        frame_rate = 0;
        videoBitRate = 0;
        fileName = "";
    }
    bool hasCamera;
    bool hasDesk;
    bool hasAudio;

    // 视频信息
    int width;
    int height;
    int frame_rate;
    int videoBitRate;
    QString fileName;
};

typedef struct OutputStream {
    AVStream* st{};
    AVCodecContext* enc{};

    int64_t next_pts = 0;
    int samples_count{};

    AVFrame* frame{};
    AVFrame* tmp_frame{};
    uint8_t* frameBuffer{};
    int frameBufferSize{};

    float t{}, tincr{}, tincr2{};

    struct SwsContext* sws_ctx{};
    struct SwrContext* swr_ctx{};
} OutputStream;

struct BufferDataNode {
    uint8_t* buffer;
    int bufferSize;
    int64_t time;  // 视频帧用于稳帧, 比较时间
};

class SaveVideoFileThread : public QThread {
    Q_OBJECT

public:
    SaveVideoFileThread();
    void run() override;
    void videoDataQueue_Input(uint8_t* buffer, int size, int64_t time);
    BufferDataNode* videoDataQueue_get(int64_t time);
    void audioDataQueue_Input(const uint8_t* buffer, const int& size);
    BufferDataNode* audioDataQueue_get();
    void add_video_stream(OutputStream* ost, AVFormatContext* oc, AVCodec** codec, enum AVCodecID codec_id);
    void add_audio_stream(OutputStream* ost, AVFormatContext* oc, AVCodec** codec, enum AVCodecID codec_id);
    void open_video(AVFormatContext* oc, AVCodec* codec, OutputStream* ost);
    void open_audio(AVFormatContext* oc, AVCodec* codec, OutputStream* ost);
    void close_stream(AVFormatContext* oc, OutputStream* ost);
    int write_frame(AVFormatContext* fmt_ctx, AVCodecContext* c, AVStream* st, AVFrame* frame);
    int write_frame(AVFormatContext* fmt_ctx, AVCodecContext* c, AVStream* st, AVFrame* frame, int64_t& pts,
                    OutputStream* ost);
    bool write_video_frame(AVFormatContext* oc, OutputStream* ost, double time);
    bool write_audio_frame(AVFormatContext* oc, OutputStream* ost);

signals:
    void SIG_sendVideoFrame(QImage img);
    void SIG_sendPicInPic(QImage img);

public slots:
    void slots_writeVideoFrameData(uint8_t* picture_buf, int buffer_size);
    void slot_writeAudioFrameData(uint8_t* picture_buf, int buffer_size);

    void slots_setInfo(STRU_AV_FORMAT av_format);

    void slots_openVideo();

    void slots_closeVideo();

private:
    VideoRecorder* m_videoRecorder = nullptr;
    AudioRecorder* m_audioRecorder = nullptr;
    STRU_AV_FORMAT m_avFormat;
    int mAudioOneFrameSize = 0;
    OutputStream video_st = {nullptr}, audio_st = {nullptr};
    AVOutputFormat* fmt;
    AVFormatContext* oc;
    AVCodec *audio_codec, *video_codec;
    int ret;
    int have_video = 0, have_audio = 0;
    int encode_video = 0, encode_audio = 0;
    QList<BufferDataNode*> m_videoDataList;
    QList<BufferDataNode*> m_audioDataList;
    BufferDataNode* lastVideoNode = nullptr;
    bool m_videoBeginFlag = false;
    qint64 m_videoBeginTime = 0;
    QMutex m_videoMutex;
    QMutex m_audioMutex;
    bool isStop = false;
    int64_t video_pts = 0;
    int64_t audio_pts = 0;
};

#endif
