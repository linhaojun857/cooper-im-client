#include "IMKernel.hpp"

#include <QMessageBox>
#include <QThread>

#include "data/DataSync.hpp"
#include "mock/Mock.hpp"
#include "store/IMStore.hpp"
#include "view/ApplyNotifyItem.hpp"

using namespace std::placeholders;

IMKernel::IMKernel(QObject* parent) {
    m_mediator = new TcpClientMediator();
    if (!m_mediator->openNet(APP_BUSINESS_TCP_SERVER_IP, APP_BUSINESS_SERVER_PORT)) {
        QMessageBox::warning(nullptr, "提示", "打开网路失败");
        exit(0);
    }
    connect(m_mediator, SIGNAL(SIG_readyData(const QJsonObject&)), this, SLOT(dealData(const QJsonObject&)));
    initHandlers();
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
}

void IMKernel::sendAuthMsg() {
    QJsonObject json;
    json["type"] = PROTOCOL_TYPE_AUTH_MSG;
    json["token"] = IMStore::getInstance()->getToken();
    m_mediator->sendData(json);
}

void IMKernel::sendSyncCompleteMsg() {
    QJsonObject json;
    json["type"] = PROTOCOL_TYPE_SYNC_COMPLETE_MESSAGE;
    json["token"] = IMStore::getInstance()->getToken();
    m_mediator->sendData(json);
}

void IMKernel::sendPersonMsg(const PersonMessage& personMessage) {
    QJsonObject json;
    json["type"] = PROTOCOL_TYPE_PERSON_MESSAGE_SEND;
    json["token"] = IMStore::getInstance()->getToken();
    json["personMessage"] = personMessage.toJson();
    m_mediator->sendData(json);
}

void IMKernel::sendGroupMsg(const GroupMessage& groupMessage) {
    QJsonObject json;
    json["type"] = PROTOCOL_TYPE_GROUP_MESSAGE_SEND;
    json["token"] = IMStore::getInstance()->getToken();
    json["groupMessage"] = groupMessage.toJson();
    m_mediator->sendData(json);
}

void IMKernel::sendLiveMsg(const QJsonObject& json) {
    m_mediator->sendData(json);
}

void IMKernel::dealData(const QJsonObject& jsonObject) {
    auto type = jsonObject["type"].toInt();
    auto it = m_handlers.find(type);
    if (it != m_handlers.end()) {
        it.value()(jsonObject);
    }
}

void IMKernel::initHandlers() {
    m_handlers[PROTOCOL_TYPE_ERROR_MSG] = std::bind(&IMKernel::handleErrorMsg, _1);
    m_handlers[PROTOCOL_TYPE_FRIEND_APPLY_NOTIFY_I] = std::bind(&IMKernel::handleFriendApplyNotifyI, _1);
    m_handlers[PROTOCOL_TYPE_FRIEND_APPLY_NOTIFY_P] = std::bind(&IMKernel::handleFriendApplyNotifyP, _1);
    m_handlers[PROTOCOL_TYPE_FRIEND_ENTITY] = std::bind(&IMKernel::handleFriendEntity, _1);
    m_handlers[PROTOCOL_TYPE_PERSON_MESSAGE_RECV] = std::bind(&IMKernel::handlePersonMessageRecv, _1);
    m_handlers[PROTOCOL_TYPE_PERSON_MESSAGE_SEND] = std::bind(&IMKernel::handlePersonMessageSend, _1);
    m_handlers[PROTOCOL_TYPE_GROUP_APPLY_NOTIFY_I] = std::bind(&IMKernel::handleGroupApplyNotifyI, _1);
    m_handlers[PROTOCOL_TYPE_GROUP_APPLY_NOTIFY_P] = std::bind(&IMKernel::handleGroupApplyNotifyP, _1);
    m_handlers[PROTOCOL_TYPE_GROUP_ENTITY] = std::bind(&IMKernel::handleGroupEntity, _1);
    m_handlers[PROTOCOL_TYPE_GROUP_MESSAGE_RECV] = std::bind(&IMKernel::handleGroupMessageRecv, _1);
    m_handlers[PROTOCOL_TYPE_GROUP_MESSAGE_SEND] = std::bind(&IMKernel::handleGroupMessageSend, _1);
    m_handlers[PROTOCOL_TYPE_LIVE_ROOM_END] = std::bind(&IMKernel::handleLiveRoomEnd, _1);
    m_handlers[PROTOCOL_TYPE_LIVE_ROOM_MSG_RECV] = std::bind(&IMKernel::handleLiveRoomMSGRecv, _1);
    m_handlers[PROTOCOL_TYPE_LIVE_ROOM_UPDATE_VIEWER_COUNT] = std::bind(&IMKernel::handleLiveRoomUpdateViewerCount, _1);
}

void IMKernel::handleErrorMsg(const QJsonObject& json) {
    QMessageBox::warning(nullptr, "提示", json["msg"].toString());
}

void IMKernel::handleFriendApplyNotifyI(const QJsonObject& json) {
    auto fa = FriendApply::fromJson(json);
    if (!IMStore::getInstance()->haveFANItemI(fa->to_id)) {
        auto fanItem = new ApplyNotifyItem();
        fanItem->setIPMode(0);
        fanItem->setFGMode(0);
        fanItem->setAvatar(fa->from_avatar);
        fanItem->setName(fa->from_nickname);
        fanItem->setOperation("请求添加对方为好友");
        fanItem->setReason(fa->reason);
        IMStore::getInstance()->addFANItemI(fa->to_id, fanItem);
        IMStore::getInstance()->getNotifyWidget()->addApplyNotifyItem(fanItem);
    } else {
        auto fanItem = IMStore::getInstance()->getFANItemI(fa->to_id);
        if (fa->agree == 1) {
            fanItem->setStatus("对方已同意");
        } else if (fa->agree == 2) {
            fanItem->setStatus("对方已拒绝");
        }
    }
}

void IMKernel::handleFriendApplyNotifyP(const QJsonObject& json) {
    auto fa = FriendApply::fromJson(json);
    IMStore::getInstance()->addFriendApplyP(fa);
    if (!IMStore::getInstance()->haveFANItemP(fa->from_id)) {
        auto fanItem = new ApplyNotifyItem();
        fanItem->setIPMode(1);
        fanItem->setFGMode(0);
        fanItem->setAppleId(fa->id);
        fanItem->setAvatar(fa->from_avatar);
        fanItem->setName(fa->from_nickname);
        fanItem->setOperation("申请添加你为好友");
        fanItem->setReason(fa->reason);
        IMStore::getInstance()->addFANItemP(fa->from_id, fanItem);
        IMStore::getInstance()->getNotifyWidget()->addApplyNotifyItem(fanItem);
    } else {
        auto fanItem = IMStore::getInstance()->getFANItemP(fa->from_id);
        fanItem->setOperation("申请添加你为好友");
        fanItem->setReason(fa->reason);
        fanItem->setIPMode(1);
        fanItem->setFGMode(0);
    }
}

void IMKernel::handleGroupApplyNotifyI(const QJsonObject& json) {
    auto ga = GroupApply::fromJson(json);
    if (!IMStore::getInstance()->haveGANItemI(ga->group_id)) {
        auto ganItem = new ApplyNotifyItem();
        ganItem->setIPMode(0);
        ganItem->setFGMode(1);
        ganItem->setAvatar(ga->to_avatar);
        ganItem->setName(ga->to_name);
        ganItem->setOperation("请求加入群聊");
        ganItem->setReason(ga->reason);
        IMStore::getInstance()->addGANItemI(ga->group_id, ganItem);
        IMStore::getInstance()->getNotifyWidget()->addApplyNotifyItem(ganItem);
    } else {
        auto ganItem = IMStore::getInstance()->getGANItemI(ga->group_id);
        if (ga->agree == 1) {
            ganItem->setStatus("已同意");
        } else if (ga->agree == 2) {
            ganItem->setStatus("已拒绝");
        }
    }
}

void IMKernel::handleGroupApplyNotifyP(const QJsonObject& json) {
    auto ga = GroupApply::fromJson(json);
    if (!IMStore::getInstance()->haveFANItemP(ga->from_id)) {
        auto ganItem = new ApplyNotifyItem();
        ganItem->setIPMode(1);
        ganItem->setFGMode(1);
        ganItem->setAppleId(ga->id);
        ganItem->setAvatar(ga->from_avatar);
        ganItem->setName(ga->from_nickname);
        ganItem->setOperation("申请加入 " + ga->to_name);
        ganItem->setReason(ga->reason);
        IMStore::getInstance()->addGANItemP(ga->from_id, ganItem);
        IMStore::getInstance()->getNotifyWidget()->addApplyNotifyItem(ganItem);
    } else {
        auto ganItem = IMStore::getInstance()->getFANItemP(ga->from_id);
        ganItem->setOperation("申请加入 " + ga->to_name);
        ganItem->setReason(ga->reason);
        ganItem->setIPMode(1);
        ganItem->setFGMode(1);
    }
}

void IMKernel::handleFriendEntity(const QJsonObject& json) {
    IMStore::getInstance()->addFriend(json);
    DataSync::syncFriendsByServerPush(json);
}

void IMKernel::handlePersonMessageRecv(const QJsonObject& json) {
    auto pm = PersonMessage::fromJson(json);
    DataSync::syncPersonMessagesBuServerPush(json);
    if (pm.from_id == IMStore::getInstance()->getChatDialog()->getCurrentPeerId()) {
        WebHelper::addPeerPersonMsg(pm);
        WebHelper::scrollToBottom();
    }
    IMStore::getInstance()->updatePersonMessageItem(pm);
}

void IMKernel::handlePersonMessageSend(const QJsonObject& json) {
    auto pm = PersonMessage::fromJson(json);
    DataSync::syncPersonMessagesBuServerPush(json);
}

void IMKernel::handleGroupEntity(const QJsonObject& json) {
    IMStore::getInstance()->addGroup(json);
}

void IMKernel::handleGroupMessageRecv(const QJsonObject& json) {
    auto gm = GroupMessage::fromJson(json);
    DataSync::syncGroupMessagesByServerPush(json);
    if (gm.group_id == IMStore::getInstance()->getChatDialog()->getCurrentGroupId()) {
        WebHelper::addPeerGroupMsg(gm);
        WebHelper::scrollToBottom();
    }
    IMStore::getInstance()->updateGroupMessageItem(gm);
}

void IMKernel::handleGroupMessageSend(const QJsonObject& json) {
    auto gm = GroupMessage::fromJson(json);
    DataSync::syncGroupMessagesByServerPush(json);
}

void IMKernel::handleLiveRoomEnd(const QJsonObject& json) {
    (void)json;
    IMStore::getInstance()->getLivePlayerDialog()->closeLive();
}

void IMKernel::handleLiveRoomMSGRecv(const QJsonObject& json) {
    auto msg = json["msg"].toString();
    auto nickname = json["nickname"].toString();
    auto avatar = json["avatar"].toString();
    auto livePlayerDialog = IMStore::getInstance()->getLivePlayerDialog();
    auto liveRoomMsgItem = new LiveRoomMsgItem();
    liveRoomMsgItem->setAvatar(avatar);
    liveRoomMsgItem->setNicknameAndMsg(nickname, msg);
    livePlayerDialog->addLiveRoomMsgItem(liveRoomMsgItem);
}

void IMKernel::handleLiveRoomUpdateViewerCount(const QJsonObject& json) {
    auto livePlayerDialog = IMStore::getInstance()->getLivePlayerDialog();
    livePlayerDialog->setLiveRoomViewerCount(json["viewer_count"].toInt());
}
