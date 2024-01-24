#include "TestAudioRWWidget.hpp"

#include "ui_TestAudioRWWidget.h"

TestAudioRWWidget::TestAudioRWWidget(QWidget* parent) : QWidget(parent), ui(new Ui::TestAudioRWWidget) {
    ui->setupUi(this);

    m_audioReader = new AudioReader();
    m_audioWriter = new AudioWriter();

    connect(m_audioReader, &AudioReader::SIG_audioFrame, m_audioWriter, &AudioWriter::slot_netRx);
    connect(ui->m_startPushButton, &QPushButton::clicked, this, [&]() {
        m_audioReader->start();
    });
    connect(ui->m_pausePushButton, &QPushButton::clicked, this, [&]() {
        m_audioReader->pause();
    });
}

TestAudioRWWidget::~TestAudioRWWidget() {
    delete ui;
    delete m_audioReader;
    delete m_audioWriter;
}
