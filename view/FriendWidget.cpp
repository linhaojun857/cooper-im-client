#include "FriendWidget.hpp"

#include <QVBoxLayout>

#include "mock/MockData.hpp"
#include "ui_FriendWidget.h"

FriendWidget::FriendWidget(QWidget* parent) : QWidget(parent), ui(new Ui::FriendWidget) {
    ui->setupUi(this);
    ui->m_scrollArea->setFrameStyle(QFrame::NoFrame);
    ui->m_scrollArea->setFrameShape(QFrame::NoFrame);
    ui->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto widget = new QWidget();
    ui->m_scrollArea->setWidget(widget);
    auto layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    widget->setLayout(layout);
    for (int i = 0; i < 20; ++i) {
        auto item = new FriendItem();
        item->setAvatar(MockData::urls[i % MockData::urls.size()]);
        item->setNickName(MockData::nicknames[i % MockData::nicknames.size()]);
        item->setStatus(QString("[%1]").arg(MockData::statuses[i % MockData::statuses.size()]));
        friendItems.append(item);
        layout->addWidget(item);
    }
    layout->addStretch();
}

FriendWidget::~FriendWidget() {
    delete ui;
}
