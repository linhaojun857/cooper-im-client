#ifndef media_call_audio_AudioReader_hpp
#define media_call_audio_AudioReader_hpp

#include <speex.h>

#include <QAudioFormat>
#include <QAudioSource>
#include <QDebug>
#include <QObject>
#include <QTimer>

#define SPEEX_QUALITY (8)

class AudioReader : public QObject {
    Q_OBJECT
public:
    explicit AudioReader(QObject* parent = nullptr);

    ~AudioReader() override;

    enum AudioState { Stop, Record, Pause };

signals:
    void SIG_audioFrame(const QByteArray& arr);

public slots:
    void start();

    void pause();

    void slot_readMore();

private:
    QAudioFormat m_format;
    QTimer* m_timer = nullptr;
    AudioState m_audioState;
    QAudioSource* m_audioSource = nullptr;
    QIODevice* m_buf = nullptr;
    SpeexBits m_bitsEnc{};
    void* m_encState = nullptr;
};

#endif
