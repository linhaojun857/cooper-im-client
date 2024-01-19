#include <QApplication>

#include "view/LivePlayerDialog.hpp"

#undef main

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    LivePlayerDialog d;
    d.show();
    return QApplication::exec();
}
