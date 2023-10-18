#ifndef core_IMKERNEL_HPP_
#define core_IMKERNEL_HPP_

#include <QObject>

#include "mediator/TcpClientMediator.hpp"
#include "view/LRWidget.hpp"
#include "view/MainWidget.hpp"

class IMKernel : public QObject {
    Q_OBJECT
public:
    using ProtocolType = int;
    using Handler = std::function<void(ProtocolType, const QJsonObject& json)>;
    explicit IMKernel(QObject* parent = nullptr);

    ~IMKernel() override;

    void createLRWidget();

    void createMainWidget();

private:
    void initHandlers();

private:
    TcpClientMediator* m_mediator = nullptr;
    LRWidget* m_lRWidget = nullptr;
    MainWidget* m_mainWidget = nullptr;
    QMap<ProtocolType, Handler> m_handlers;
};

#endif
