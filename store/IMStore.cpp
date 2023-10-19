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

void IMStore::setSelf(const QJsonObject& json) {
    qDebug() << "IMStore::setSelf";
    m_self = Self::fromJson(json["user"].toObject());
    m_mainWidget->setAvatar(m_self->avatar);
    m_mainWidget->setNickname(m_self->nickname);
    m_mainWidget->setStatusAndFeeling(m_self->status, m_self->feeling);
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

FGSWidget* IMStore::getFGSWidget() {
    return m_fgsWidget;
}
