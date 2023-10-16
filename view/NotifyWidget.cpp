#include "NotifyWidget.hpp"

#include <QPushButton>

#include "mock/Mock.hpp"
#include "ui_NotifyWidget.h"

NotifyWidget::NotifyWidget(QWidget* parent) : QWidget(parent), ui(new Ui::NotifyWidget) {
    // mock
    Mock::notifyWidget = this;
    ui->setupUi(this);
    ui->m_scrollArea->setFrameStyle(QFrame::NoFrame);
    ui->m_scrollArea->setFrameShape(QFrame::NoFrame);
    ui->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto widget = new QWidget();
    ui->m_scrollArea->setWidget(widget);
    m_layout = new QVBoxLayout();
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    widget->setLayout(m_layout);
    m_layout->addStretch();
}

NotifyWidget::~NotifyWidget() {
    delete ui;
}

void NotifyWidget::addNewFriendItem(NewFriendItem* item) {
    qDebug() << "NotifyWidget::addNewFriItem";
    m_newFriendItems.append(item);
    m_layout->insertWidget(0, item);
}
