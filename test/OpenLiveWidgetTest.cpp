#include <QApplication>

#include "store/IMStore.hpp"
#include "view/OpenLiveWidget.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    IMStore::getInstance()->setToken(
        "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjEiLCJpc3MiOiJjb29wZXIifQ.ffNkmzByo3zVk_t3NyG9n5iR_"
        "XUemazSPSSOKeqvyM8");
    OpenLiveWidget w;
    w.show();
    return QApplication::exec();
}
