#include "LiveLobbyWidget.hpp"

#include <QPushButton>

#include "define/IMDefine.hpp"
#include "store/IMStore.hpp"
#include "ui_LiveLobbyWidget.h"
#include "util/HttpUtil.hpp"

LiveLobbyWidget::LiveLobbyWidget(QWidget* parent) : QWidget(parent), ui(new Ui::LiveLobbyWidget) {
    ui->setupUi(this);
    setWindowTitle("Cooper直播大厅");
    setWindowIcon(QIcon(":/img/logo.ico"));

    connect(ui->m_searchPushButton, &QPushButton::clicked, this, &LiveLobbyWidget::handleClickSearchPushButton);
    connect(ui->m_settingPushButton, &QPushButton::clicked, this, &LiveLobbyWidget::handleClickSettingPushButton);
    connect(ui->m_openLivePushButton, &QPushButton::clicked, this, &LiveLobbyWidget::handleClickOpenLivePushButton);

    ui->m_scrollArea->setFrameStyle(QFrame::NoFrame);
    ui->m_scrollArea->setFrameShape(QFrame::NoFrame);
    ui->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto widget = new QWidget();
    ui->m_scrollArea->setWidget(widget);
    m_liveRoomItemsLayout = new QGridLayout();
    m_liveRoomItemsLayout->setContentsMargins(0, 0, 0, 0);
    m_liveRoomItemsLayout->setSpacing(0);
    m_liveRoomItemsLayout->setAlignment(Qt::AlignTop);
    m_liveRoomItemsLayout->setColumnMinimumWidth(0, 250);
    m_liveRoomItemsLayout->setColumnMinimumWidth(1, 250);
    m_liveRoomItemsLayout->setColumnMinimumWidth(2, 250);
    widget->setLayout(m_liveRoomItemsLayout);

    m_openLiveWidget = new OpenLiveWidget();
}

LiveLobbyWidget::~LiveLobbyWidget() {
    delete ui;
    delete m_openLiveWidget;
}

void LiveLobbyWidget::addLiveRoomItem(LiveRoomItem* item, int row, int column) {
    m_liveRoomItemsLayout->addWidget(item, row, column);
    m_liveRoomItems.append(item);
}

void LiveLobbyWidget::clearLiveRoomItems() {
    QLayoutItem* child;
    while ((child = m_liveRoomItemsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    m_liveRoomItems.clear();
}

void LiveLobbyWidget::flushLiveRooms() {
    clearLiveRoomItems();
    auto ret = HttpUtil::get(HTTP_SERVER_URL "/live/getOpenedLives");
    if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
        return;
    }
    auto liveRooms = ret["live_rooms"].toArray();
    int index = 0;
    for (auto&& liveRoom : liveRooms) {
        auto* item = new LiveRoomItem();
        item->setLiveRoomId(liveRoom.toObject()["id"].toInt());
        item->setLiveRoomCover(liveRoom.toObject()["cover"].toString());
        item->setLiveRoomOwnerAvatar(liveRoom.toObject()["owner_avatar"].toString());
        item->setLiveRoomOwnerName(liveRoom.toObject()["owner_nickname"].toString());
        item->setLiveRoomViewerCount(liveRoom.toObject()["viewer_count"].toInt());
        addLiveRoomItem(item, index / 3, index % 3);
        index++;
    }
}

void LiveLobbyWidget::handleClickSearchPushButton() {
}

void LiveLobbyWidget::handleClickSettingPushButton() {
}

void LiveLobbyWidget::handleClickOpenLivePushButton() {
    m_openLiveWidget->reset();
    m_openLiveWidget->show();
}
