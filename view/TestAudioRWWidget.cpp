#include "TestAudioRWWidget.hpp"

#include "ui_TestAudioRWWidget.h"

TestAudioRWWidget::TestAudioRWWidget(QWidget* parent) : QWidget(parent), ui(new Ui::TestAudioRWWidget) {
    ui->setupUi(this);

    setWindowIcon(QIcon(":/img/logo.ico"));
    setWindowTitle("测试音频读写");

    m_audioReader = new AudioReader();
    m_audioWriter = new AudioWriter();

    connect(m_audioReader, &AudioReader::SIG_audioFrame, m_audioWriter, &AudioWriter::slot_netRx);
    connect(ui->m_startPushButton, &QPushButton::clicked, this, [&]() {
        m_audioReader->openAudio();
    });
    connect(ui->m_pausePushButton, &QPushButton::clicked, this, [&]() {
        m_audioReader->pauseAudio();
    });
}

TestAudioRWWidget::~TestAudioRWWidget() {
    delete ui;
    delete m_audioReader;
    delete m_audioWriter;
}
