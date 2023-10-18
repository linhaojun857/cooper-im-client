#include <QApplication>

#include "core/IMKernel.hpp"
#include "mock/Mock.hpp"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    IMKernel kernel;
    Mock::InitMockData();
    return QApplication::exec();
}
