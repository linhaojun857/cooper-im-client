#include "IMKernel.hpp"

#include <QMessageBox>
#include <QThread>

#include "data/DataSync.hpp"
#include "mock/Mock.hpp"
#include "store/IMStore.hpp"
#include "view/FANItem.hpp"

using namespace std::placeholders;

IMKernel::IMKernel(QObject* parent) {
    m_mediator = new TcpClientMediator();
    if (!m_mediator->openNet()) {
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
    Mock::addMockGroupItems();
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

void IMKernel::dealData(const QJsonObject& jsonObject) {
    auto type = jsonObject["type"].toInt();
    auto it = m_handlers.find(type);
    if (it != m_handlers.end()) {
        it.value()(jsonObject);
    }
}

void IMKernel::initHandlers() {
    m_handlers[PROTOCOL_TYPE_ERROR_MSG] = std::bind(&IMKernel::handleErrorMsg, _1);
    m_handlers[PROTOCOL_TYPE_FRIEND_APPLY_NOTIFY_I] = std::bind(&IMKernel::handleFriendAppleNotifyI, _1);
    m_handlers[PROTOCOL_TYPE_FRIEND_APPLY_NOTIFY_P] = std::bind(&IMKernel::handleFriendAppleNotifyP, _1);
    m_handlers[PROTOCOL_TYPE_FRIEND_ENTITY] = std::bind(&IMKernel::handleFriendEntity, _1);
    m_handlers[PROTOCOL_TYPE_PERSON_MESSAGE_RECV] = std::bind(&IMKernel::handlePersonMessageRecv, _1);
    m_handlers[PROTOCOL_TYPE_PERSON_MESSAGE_SEND] = std::bind(&IMKernel::handlePersonMessageSend, _1);
}

void IMKernel::handleErrorMsg(const QJsonObject& json) {
    QMessageBox::warning(nullptr, "提示", json["msg"].toString());
}

void IMKernel::handleFriendAppleNotifyI(const QJsonObject& json) {
    auto fa = FriendApply::fromJson(json);
    auto fanItem = IMStore::getInstance()->getFANItemI(fa->to_id);
    if (fanItem == nullptr) {
        fanItem = new FANItem();
        fanItem->setMode(0);
        fanItem->setAvatar(fa->from_avatar);
        fanItem->setNickname(fa->from_nickname);
        fanItem->setReason("请求添加对方为好友");
        IMStore::getInstance()->addFANItemI(fa->to_id, fanItem);
        IMStore::getInstance()->getNotifyWidget()->addFANItem(fanItem);
    }
    if (fa->agree == 1) {
        fanItem->setStatus("对方已同意");
    } else if (fa->agree == 2) {
        fanItem->setStatus("对方已拒绝");
    }
}

void IMKernel::handleFriendAppleNotifyP(const QJsonObject& json) {
    auto fa = FriendApply::fromJson(json);
    IMStore::getInstance()->addFriendApplyP(fa);
    if (!IMStore::getInstance()->haveFANItemP(fa->from_id)) {
        auto fanItem = new FANItem();
        fanItem->setMode(1);
        fanItem->setFromId(fa->from_id);
        fanItem->setAvatar(fa->from_avatar);
        fanItem->setNickname(fa->from_nickname);
        fanItem->setReason(fa->reason);
        IMStore::getInstance()->getNotifyWidget()->addFANItem(fanItem);
        IMStore::getInstance()->addFANItemP(fa->from_id, fanItem);
    } else {
        auto fanItem = IMStore::getInstance()->getFANItemP(fa->from_id);
        fanItem->setReason(fa->reason);
        fanItem->setMode(1);
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
        WebHelper::addPeerTextMsg(pm.from_id, pm.message);
        WebHelper::scrollToBottom();
    }
    IMStore::getInstance()->updatePersonMessageItem(pm);
}

void IMKernel::handlePersonMessageSend(const QJsonObject& json) {
    auto pm = PersonMessage::fromJson(json);
    DataSync::syncPersonMessagesBuServerPush(json);
}
