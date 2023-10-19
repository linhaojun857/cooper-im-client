#include <QApplication>

#include "mock/Mock.hpp"
#include "view/FGSWidget.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    FGSWidget w;
    w.show();
    Mock::addMockFGSItems();
    return QApplication::exec();
}
