#ifndef media_call_audio_AudioWriter_hpp
#define media_call_audio_AudioWriter_hpp

#include <speex.h>

#include <QAudioFormat>
#include <QAudioSink>
#include <QDebug>
#include <QObject>

class AudioWriter : public QObject {
    Q_OBJECT
public:
    explicit AudioWriter(QObject* parent = nullptr);

    ~AudioWriter() override;

public slots:
    void slot_netRx(const QByteArray& arr);

private:
    QAudioFormat m_format;
    QAudioSink* m_audioSink = nullptr;
    QIODevice* m_buf = nullptr;
    SpeexBits m_bitsDec{};
    void* m_decState = nullptr;
};

#endif
