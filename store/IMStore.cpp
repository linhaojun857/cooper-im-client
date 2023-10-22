#include "IMStore.hpp"

#include <QJsonArray>
#include <QJsonObject>
#include <QThread>

IMStore* IMStore::getInstance() {
    static IMStore store;
    return &store;
}

IMStore::IMStore() {
    m_chatDialog = new ChatDialog();
    m_fgsWidget = new FGSWidget();
}

void IMStore::setIMKernel(IMKernel* imKernel) {
    m_imKernel = imKernel;
}

IMKernel* IMStore::getIMKernel() {
    return m_imKernel;
}

void IMStore::setMainWidget(MainWidget* mainWidget) {
    m_mainWidget = mainWidget;
}

MainWidget* IMStore::getMainWidget() {
    return m_mainWidget;
}

ChatDialog* IMStore::getChatDialog() {
    return m_chatDialog;
}

void IMStore::openChatPage(int id) {
    m_openChatPageIds.insert(id);
}

void IMStore::closeChatPage(int id) {
    m_openChatPageIds.remove(id);
}

bool IMStore::isOpenChatPage(int id) {
    return m_openChatPageIds.contains(id);
}

void IMStore::setFriendWidget(FriendWidget* friendWidget) {
    m_friendWidget = friendWidget;
}

void IMStore::setNotifyWidget(NotifyWidget* notifyWidget) {
    m_notifyWidget = notifyWidget;
}

NotifyWidget* IMStore::getNotifyWidget() {
    return m_notifyWidget;
}

void IMStore::setSelf(const QJsonObject& json) {
    qDebug() << "IMStore::setSelf";
    m_self = Self::fromJson(json["user"].toObject());
    m_mainWidget->setAvatar(m_self->avatar);
    m_mainWidget->setNickname(m_self->nickname);
    m_mainWidget->setStatusAndFeeling(m_self->status, m_self->feeling);
}

Self* IMStore::getSelf() {
    return m_self;
}

void IMStore::setToken(const QJsonObject& json) {
    qDebug() << "IMStore::setToken";
    m_token = json["token"].toString();
}

QString IMStore::getToken() {
    return m_token;
}

void IMStore::addFriend(const QJsonObject& json) {
    qDebug() << "IMStore::addFriend";
    auto fri = Friend::fromJson(json);
    auto friendItem = new FriendItem();
    friendItem->setId(fri->id);
    friendItem->setNickName(fri->nickname);
    friendItem->setAvatar(fri->avatar);
    friendItem->setStatusAndFeeling(fri->status, fri->feeling);
    m_friends[fri->id] = fri;
    m_friendItems[fri->id] = friendItem;
    m_friendWidget->addFriendItem(friendItem);
}

void IMStore::addFriends(const QJsonObject& json) {
    qDebug() << "IMStore::addFriends";
    QJsonArray friends = json["friends"].toArray();
    for (const auto& f : friends) {
        auto obj = f.toObject();
        auto fri = Friend::fromJson(obj);
        auto friendItem = new FriendItem();
        friendItem->setId(fri->id);
        friendItem->setNickName(fri->nickname);
        friendItem->setAvatar(fri->avatar);
        friendItem->setStatusAndFeeling(fri->status, fri->feeling);
        m_friends[fri->id] = fri;
        m_friendItems[fri->id] = friendItem;
        m_friendWidget->addFriendItem(friendItem);
    }
}

FGSWidget* IMStore::getFGSWidget() const {
    return m_fgsWidget;
}

void IMStore::addFSRs(const QJsonObject& json) {
    qDebug() << "IMStore::addFSRs";
    QJsonArray fsrs = json["fsrs"].toArray();
    int row;
    int column;
    for (int i = 0; i < fsrs.size(); ++i) {
        row = i / 3;
        column = i % 3;
        auto fsrItem = new FSRItem();
        auto obj = fsrs[i].toObject();
        fsrItem->setId(obj["id"].toInt());
        fsrItem->setAvatar(obj["avatar"].toString());
        fsrItem->setNickname(obj["nickname"].toString());
        m_fgsWidget->addFSRItem(fsrItem, row, column);
    }
}

void IMStore::addFriendApplyI(FriendApply* friendApply) {
    m_friendApplyIs.append(friendApply);
}

void IMStore::addFANItemI(int id, FANItem* fanItem) {
    m_fanItemIs[id] = fanItem;
}

bool IMStore::haveFANItemI(int id) {
    return m_fanItemIs.contains(id);
}

FANItem* IMStore::getFANItemI(int id) {
    return m_fanItemIs[id];
}

void IMStore::addFriendApplyP(FriendApply* friendApply) {
    m_friendApplyPs.append(friendApply);
}

void IMStore::addFANItemP(int id, FANItem* fanItem) {
    m_fanItemPs[id] = fanItem;
}

bool IMStore::haveFANItemP(int id) {
    return m_fanItemPs.contains(id);
}

FANItem* IMStore::getFANItemP(int id) {
    return m_fanItemPs[id];
}
