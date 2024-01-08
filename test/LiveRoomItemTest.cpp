#include <QApplication>

#include "view/LiveRoomItem.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    LiveRoomItem w;
    w.show();
    return QApplication::exec();
}
