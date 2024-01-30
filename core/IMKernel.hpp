#ifndef core_IMKERNEL_HPP_
#define core_IMKERNEL_HPP_

#include <QObject>

#include "define/IMDefine.hpp"
#include "entity/Entity.hpp"
#include "mediator/TcpClientMediator.hpp"
#include "view/LRWidget.hpp"
#include "view/MainWidget.hpp"
#include "view/VideoCallRequestDialog.hpp"

class IMKernel : public QObject {
    Q_OBJECT
public:
    using ProtocolType = int;
    using BusinessHandler = std::function<void(const QJsonObject& json)>;
    using MediaHandler = std::function<void(char* buf, int size)>;

    explicit IMKernel(QObject* parent = nullptr);

    ~IMKernel() override;

    void createLRWidget();

    void createMainWidget();

    void sendAuthMsg();

    void sendSyncCompleteMsg();

    void sendPersonMsg(const PersonMessage& personMessage);

    void sendGroupMsg(const GroupMessage& groupMessage);

    void sendLiveMsg(const QJsonObject& json);

    void sendVideoCallRequest(const QJsonObject& json);

    void sendVideoCallResponse(const QJsonObject& json);

    void openAVCall();

    void closeAVCall();

    TcpClientMediator* getAVCallMediator();

public slots:
    void dealBusinessData(const QJsonObject& jsonObject);

    void dealMediaData(char* buf, int size);

private:
    void initHandlers();

    static void handleErrorMsg(const QJsonObject& json);

    static void handleFriendApplyNotifyI(const QJsonObject& json);

    static void handleFriendApplyNotifyP(const QJsonObject& json);

    static void handleGroupApplyNotifyI(const QJsonObject& json);

    static void handleGroupApplyNotifyP(const QJsonObject& json);

    static void handleFriendEntity(const QJsonObject& json);

    static void handlePersonMessageRecv(const QJsonObject& json);

    static void handlePersonMessageSend(const QJsonObject& json);

    static void handleGroupEntity(const QJsonObject& json);

    static void handleGroupMessageRecv(const QJsonObject& json);

    static void handleGroupMessageSend(const QJsonObject& json);

    static void handleLiveRoomEnd(const QJsonObject& json);

    static void handleLiveRoomMSGRecv(const QJsonObject& json);

    static void handleLiveRoomUpdateViewerCount(const QJsonObject& json);

    static void handleVideoCallRequest(const QJsonObject& json);

    static void handleVideoCallResponse(const QJsonObject& json);

    static void handleVideoCallAudioFrame(char* buf, int size);

    static void handleVideoCallVideoFrame(char* buf, int size);

    static void handleVideoCallEnd(const QJsonObject& json);

private:
    TcpClientMediator* m_mediator = nullptr;
    TcpClientMediator* m_avCallMediator = nullptr;
    LRWidget* m_lRWidget = nullptr;
    MainWidget* m_mainWidget = nullptr;
    QMap<ProtocolType, BusinessHandler> m_businessHandlers;
    QMap<ProtocolType, MediaHandler> m_mediaHandlers;
};

#endif
