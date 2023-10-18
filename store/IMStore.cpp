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
}

void IMStore::setIMKernel(IMKernel* imKernel) {
    m_imKernel = imKernel;
}

IMKernel* IMStore::getIMKernel() {
    return m_imKernel;
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

void IMStore::addFriends(const QJsonObject& json) {
    QJsonArray friends = json["friends"].toArray();
    for (const auto& f : friends) {
        auto obj = f.toObject();
        int id = obj["id"].toInt();
        auto username = obj["username"].toString();
        auto nickname = obj["nickname"].toString();
        auto avatar = obj["avatar"].toString();
        auto status = obj["status"].toString();
        auto feeling = obj["feeling"].toString();
        auto friendItem = new FriendItem();
        friendItem->setId(id);
        friendItem->setNickName(nickname);
        friendItem->setAvatar(avatar);
        friendItem->setStatus(status);
        friendItem->setFeeling(feeling);
        auto fri = new Friend(id, username, nickname, avatar, status, feeling);
        m_friends[id] = fri;
        m_friendItems[id] = friendItem;
        m_friendWidget->addFriendItem(friendItem);
    }
}
