#ifndef view_VideoCallDialog_hpp
#define view_VideoCallDialog_hpp

#include <QMutex>
#include <QWidget>

#include "core/IMKernel.hpp"
#include "media/call/audio/AudioReader.hpp"
#include "media/call/audio/AudioWriter.hpp"
#include "media/call/video/VideoReader.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class VideoCallDialog;
}
QT_END_NAMESPACE

class VideoCallDialog : public QWidget {
    Q_OBJECT

public:
    explicit VideoCallDialog(QWidget* parent = nullptr);

    ~VideoCallDialog() override;

    void setPeerId(int peerId);

    void setAvatar(const QString& url);

    void showMessageBox(const QString& content);

    void startCall();

    void closeCall();

    void playAudio(const QByteArray& arr);

    void playVideo(const QImage& image);

public slots:
    void handleAudioFrame(const QByteArray& arr);

    void handleVideoFrame(const QImage& image);

private:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::VideoCallDialog* ui;
    int m_peerId = 0;
    TcpClientMediator* m_mediator = nullptr;
    QBasicMutex m_mutex;
    AudioReader* m_audioReader = nullptr;
    AudioWriter* m_audioWriter = nullptr;
    VideoReader* m_videoReader = nullptr;
    bool m_adjustFlag = true;
};

#endif
