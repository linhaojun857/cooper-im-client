#include "ChatDialog.hpp"

#include <FramelessHelper/Widgets/framelesswidgetshelper.h>

#include "ui_ChatDialog.h"

FRAMELESSHELPER_USE_NAMESPACE

ChatDialog::ChatDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
    m_headerWidget = new QWidget(this);
    m_headerWidget->setGeometry(0, 0, 450, 30);
    m_headerWidget->setStyleSheet("QWidget{border-bottom: 1px solid #000000;}");
    ui->m_peerNicknameLabel->setStyleSheet("border-bottom: 0px;");
    ui->m_peerNicknameLabel->setParent(m_headerWidget);
    ui->m_closePushButton->setParent(m_headerWidget);
    ui->m_minimizePushButton->setParent(m_headerWidget);
    connect(ui->m_closePushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->m_minimizePushButton, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));

    FramelessWidgetsHelper::get(this)->extendsContentIntoTitleBar();
    FramelessWidgetsHelper::get(this)->setTitleBarWidget(m_headerWidget);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(ui->m_closePushButton);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(ui->m_minimizePushButton);
}

ChatDialog::~ChatDialog() {
    delete ui;
}
