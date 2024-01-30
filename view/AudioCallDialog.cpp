#include "AudioCallDialog.hpp"

#include "ui_AudioCallDialog.h"

AudioCallDialog::AudioCallDialog(QWidget* parent) : QWidget(parent), ui(new Ui::AudioCallDialog) {
    ui->setupUi(this);
}

AudioCallDialog::~AudioCallDialog() {
    delete ui;
}
