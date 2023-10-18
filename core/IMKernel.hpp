#ifndef core_IMKERNEL_HPP_
#define core_IMKERNEL_HPP_

#include <QObject>

#include "mediator/TcpClientMediator.hpp"
#include "view/LRWidget.hpp"
#include "view/MainWidget.hpp"

class IMKernel : public QObject {
    Q_OBJECT
public:
    explicit IMKernel(QObject* parent = nullptr);

    ~IMKernel() override;

    void createLRWidget();

    void createMainWidget();

private:
    TcpClientMediator* m_mediator = nullptr;
    LRWidget* m_lRWidget = nullptr;
    MainWidget* m_mainWidget = nullptr;
};

#endif
