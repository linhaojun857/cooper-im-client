#include "IMKernel.hpp"

#include <QThread>

#include "mock/Mock.hpp"
#include "store/IMStore.hpp"

IMKernel::IMKernel(QObject* parent) {
    IMStore::getInstance()->setIMKernel(this);
    createLRWidget();
}

IMKernel::~IMKernel() {
    delete m_lRWidget;
    delete m_mainWidget;
}

void IMKernel::createLRWidget() {
    m_lRWidget = new LRWidget();
    m_lRWidget->show();
}

void IMKernel::createMainWidget() {
    m_mainWidget = new MainWidget();
    m_mainWidget->show();
    Mock::addMockGroupItems();
    Mock::addMockNewFriendItems();
}
