#include "IMKernel.hpp"

// test
#include "view/ChatDialog.hpp"

IMKernel::IMKernel(QObject* parent) {
    //    m_lRWidget = new LRWidget();
    //    m_mainWidget = new MainWidget();
    //    m_lRWidget->setMainWidget(m_mainWidget);
    //    m_lRWidget->show();
    auto test = new ChatDialog();
    test->show();
}

IMKernel::~IMKernel() {
    delete m_lRWidget;
    delete m_mainWidget;
}
