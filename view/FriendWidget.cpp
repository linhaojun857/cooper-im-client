#include "FriendWidget.hpp"

#include "mock/Mock.hpp"
#include "ui_FriendWidget.h"

FriendWidget::FriendWidget(QWidget* parent) : QWidget(parent), ui(new Ui::FriendWidget) {
    // mock
    Mock::friendWidget = this;
    ui->setupUi(this);
    ui->m_scrollArea->setFrameStyle(QFrame::NoFrame);
    ui->m_scrollArea->setFrameShape(QFrame::NoFrame);
    ui->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto widget = new QWidget();
    ui->m_scrollArea->setWidget(widget);
    m_friendItemLayout = new QVBoxLayout();
    m_friendItemLayout->setContentsMargins(0, 0, 0, 0);
    m_friendItemLayout->setSpacing(0);
    widget->setLayout(m_friendItemLayout);
    m_friendItemLayout->addStretch();
}

FriendWidget::~FriendWidget() {
    delete ui;
}

void FriendWidget::addFriendItem(FriendItem* friendItem) {
    qDebug() << "FriendWidget::addFriendItem";
    m_friendItems.append(friendItem);
    m_friendItemLayout->insertWidget(m_friendItemLayout->count() - 1, friendItem);
}
