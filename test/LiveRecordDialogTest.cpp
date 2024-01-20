#include <QApplication>

#include "view/LiveRecordDialog.hpp"

#undef main

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    LiveRecordDialog d;
    d.show();
    return QApplication::exec();
}
