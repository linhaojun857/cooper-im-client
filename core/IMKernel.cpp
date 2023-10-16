#include "IMKernel.hpp"

#include "store/IMStore.hpp"

IMKernel::IMKernel(QObject* parent) {
    IMStore::getInstance()->setIMKernel(this);
    m_lRWidget = new LRWidget();
    m_mainWidget = new MainWidget();
    m_lRWidget->setMainWidget(m_mainWidget);
    m_lRWidget->show();
}

IMKernel::~IMKernel() {
    delete m_lRWidget;
    delete m_mainWidget;
}
