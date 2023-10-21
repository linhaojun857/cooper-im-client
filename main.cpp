#include <QApplication>

#include "core/IMKernel.hpp"
#include "mock/Mock.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    Mock::InitMockData();
    IMKernel kernel;
    return QApplication::exec();
}
