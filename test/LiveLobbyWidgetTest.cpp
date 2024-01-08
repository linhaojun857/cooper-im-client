#include <QApplication>

#include "view/LiveLobbyWidget.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    LiveLobbyWidget w;
    for (int i = 0; i < 20; ++i) {
        auto item = new LiveRoomItem();
        item->setLiveRoomOwnerName("用户" + QString::number(i));
        w.addLiveRoomItem(item, i / 3, i % 3);
    }
    w.show();
    return QApplication::exec();
}
