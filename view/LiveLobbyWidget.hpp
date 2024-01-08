#ifndef view_LiveLobbyWidget_hpp
#define view_LiveLobbyWidget_hpp

#include <QGridLayout>
#include <QWidget>

#include "view/LiveRoomItem.hpp"
#include "view/OpenLiveWidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class LiveLobbyWidget;
}
QT_END_NAMESPACE

class LiveLobbyWidget : public QWidget {
    Q_OBJECT

public:
    explicit LiveLobbyWidget(QWidget* parent = nullptr);

    ~LiveLobbyWidget() override;

    void addLiveRoomItem(LiveRoomItem* item, int row, int column);

    void clearLiveRoomItems();

    void flushLiveRooms();

public slots:
    void handleClickSearchPushButton();

    void handleClickSettingPushButton();

    void handleClickOpenLivePushButton();

private:
    Ui::LiveLobbyWidget* ui;
    QList<LiveRoomItem*> m_liveRoomItems;
    QGridLayout* m_liveRoomItemsLayout = nullptr;
    OpenLiveWidget* m_openLiveWidget = nullptr;
};

#endif
