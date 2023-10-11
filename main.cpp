#include <QApplication>

#include "core/IMKernel.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    IMKernel kernel;
    return QApplication::exec();
}
