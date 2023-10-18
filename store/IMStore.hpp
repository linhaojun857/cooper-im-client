#ifndef store_IMStore_hpp
#define store_IMStore_hpp

#include <QWebEngineView>

#include "entity/Entity.hpp"
#include "view/ChatDialog.hpp"
#include "view/FriendItem.hpp"
#include "view/FriendWidget.hpp"

class IMKernel;

class IMStore {
public:
    static IMStore* getInstance();

    IMStore();

    void setIMKernel(IMKernel* imKernel);

    IMKernel* getIMKernel();

    ChatDialog* getChatDialog();

    void openChatPage(int id);

    void closeChatPage(int id);

    bool isOpenChatPage(int id);

    void setFriendWidget(FriendWidget* friendWidget);

    void addFriends(const QJsonObject& json);

private:
    IMKernel* m_imKernel = nullptr;
    ChatDialog* m_chatDialog = nullptr;
    QSet<int> m_openChatPageIds;
    FriendWidget* m_friendWidget = nullptr;
    QMap<int, Friend*> m_friends;
    QMap<int, FriendItem*> m_friendItems;
};

#endif
