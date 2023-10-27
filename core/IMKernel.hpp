#ifndef core_IMKERNEL_HPP_
#define core_IMKERNEL_HPP_

#include <QObject>

#include "define/IMDefine.hpp"
#include "entity/Entity.hpp"
#include "mediator/TcpClientMediator.hpp"
#include "view/LRWidget.hpp"
#include "view/MainWidget.hpp"

class IMKernel : public QObject {
    Q_OBJECT
public:
    using ProtocolType = int;
    using Handler = std::function<void(const QJsonObject& json)>;
    explicit IMKernel(QObject* parent = nullptr);

    ~IMKernel() override;

    void createLRWidget();

    void createMainWidget();

    void sendAuthMsg();

    void sendSyncCompleteMsg();

    void sendPersonMsg(const PersonMessage* personMessage);

public slots:
    void dealData(const QJsonObject& jsonObject);

private:
    void initHandlers();

    void handleErrorMsg(const QJsonObject& json);

    void handleFriendAppleNotifyI(const QJsonObject& json);

    void handleFriendAppleNotifyP(const QJsonObject& json);

    void handleFriendEntity(const QJsonObject& json);

private:
    TcpClientMediator* m_mediator = nullptr;
    LRWidget* m_lRWidget = nullptr;
    MainWidget* m_mainWidget = nullptr;
    QMap<ProtocolType, Handler> m_handlers;
};

#endif
