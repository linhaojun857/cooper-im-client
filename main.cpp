#include <QApplication>

#include "view/LRWidget.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    auto lRWidget = new LRWidget();
    lRWidget->show();
    return QApplication::exec();
}
