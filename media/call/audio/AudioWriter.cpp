#include "AudioWriter.hpp"

AudioWriter::AudioWriter(QObject* parent) : QObject(parent) {
    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleFormat(QAudioFormat::Int16);

    m_audioSink = new QAudioSink(m_format);
    m_buf = m_audioSink->start();

    speex_bits_init(&m_bitsDec);
    m_decState = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_NB));
}

AudioWriter::~AudioWriter() {
    if (m_audioSink) {
        m_audioSink->stop();
        delete m_audioSink;
    }
    speex_decoder_destroy(m_decState);
    speex_bits_destroy(&m_bitsDec);
}

void AudioWriter::slot_netRx(const QByteArray& arr) {
    char bytes[800] = {0};
    int i;
    float output_frame1[320] = {0};
    char buf[800] = {0};
    memcpy(bytes, arr.data(), arr.length() / 2);
    speex_bits_read_from(&m_bitsDec, bytes, (int)(arr.length() / 2));
    speex_decode(m_decState, &m_bitsDec, output_frame1);
    short num;
    for (i = 0; i < 160; i++) {
        num = (short)(output_frame1[i]);
        buf[2 * i] = (char)num;
        buf[2 * i + 1] = (char)(num >> 8);
    }
    memcpy(bytes, arr.data() + arr.length() / 2, arr.length() / 2);
    speex_bits_read_from(&m_bitsDec, bytes, (int)(arr.length() / 2));
    speex_decode(m_decState, &m_bitsDec, output_frame1);
    for (i = 0; i < 160; i++) {
        num = (short)(output_frame1[i]);
        buf[2 * i + 320] = (char)num;
        buf[2 * i + 1 + 320] = (char)(num >> 8);
    }
    m_buf->write(buf, 640);
}
