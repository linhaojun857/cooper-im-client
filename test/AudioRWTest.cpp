#include <QApplication>
#include <QThread>

#include "view/TestAudioRWWidget.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    TestAudioRWWidget test;
    test.show();
    QApplication::exec();
    return 0;
}
