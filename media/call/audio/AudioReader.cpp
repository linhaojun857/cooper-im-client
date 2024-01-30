#include "AudioReader.hpp"

AudioReader::AudioReader(QObject* parent) : QObject(parent) {
    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleFormat(QAudioFormat::Int16);

    m_audioState = Stop;

    speex_bits_init(&m_bitsEnc);
    m_encState = speex_encoder_init(speex_lib_get_mode(SPEEX_MODEID_NB));
    int tmp = SPEEX_QUALITY;
    speex_encoder_ctl(m_encState, SPEEX_SET_QUALITY, &tmp);
}

AudioReader::~AudioReader() {
    if (m_audioSource) {
        m_audioSource->stop();
        delete m_audioSource;
    }
    if (m_timer) {
        m_timer->stop();
        delete m_timer;
    }
    speex_encoder_destroy(m_encState);
    speex_bits_destroy(&m_bitsEnc);
}

void AudioReader::openAudio() {
    if (m_audioState != Record) {
        m_audioSource = new QAudioSource(m_format);
        m_buf = m_audioSource->start();
        m_timer = new QTimer();
        connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_readMore()));
        m_timer->start(20);
    }
    m_audioState = Record;
}

void AudioReader::pauseAudio() {
    if (m_audioState == Record) {
        if (m_audioSource) {
            m_audioSource->stop();
            delete m_audioSource;
            m_audioSource = nullptr;
        }
        if (m_timer) {
            m_timer->stop();
            delete m_timer;
            m_timer = nullptr;
        }
    }
    m_audioState = Pause;
}

void AudioReader::slot_readMore() {
    if (!m_audioSource) {
        return;
    }
    QByteArray arr(2048, 0);
    qint64 len = m_buf->bytesAvailable();
    if (len < 640) {
        return;
    }
    m_buf->read(arr.data(), 640);
    QByteArray frame;
    char bytes[800] = {0};
    int i;
    float input_frame1[320];
    char* pInData = (char*)arr.data();
    char buf[800] = {0};
    memcpy(buf, pInData, 640);
    int nBytes;
    short num;
    for (i = 0; i < 160; i++) {
        num = (short)((uint8_t)buf[2 * i] | (((uint8_t)buf[2 * i + 1]) << 8));
        input_frame1[i] = num;
    }
    speex_bits_reset(&m_bitsEnc);
    speex_encode(m_encState, input_frame1, &m_bitsEnc);
    nBytes = speex_bits_write(&m_bitsEnc, bytes, 800);
    frame.append(bytes, nBytes);
    for (i = 0; i < 160; i++) {
        num = (short)((uint8_t)buf[2 * i + 320] | (((uint8_t)buf[2 * i + 1 + 320]) << 8));
        input_frame1[i] = num;
    }
    speex_bits_reset(&m_bitsEnc);
    speex_encode(m_encState, input_frame1, &m_bitsEnc);
    nBytes = speex_bits_write(&m_bitsEnc, bytes, 800);
    frame.append(bytes, nBytes);
    Q_EMIT SIG_audioFrame(frame);
}
