#ifndef store_IMStore_hpp
#define store_IMStore_hpp

#include <QWebEngineView>

#include "entity/Entity.hpp"
#include "view/ChatDialog.hpp"
#include "view/FGSWidget.hpp"
#include "view/FriendItem.hpp"
#include "view/FriendWidget.hpp"
#include "view/MainWidget.hpp"

class IMKernel;

class IMStore {
public:
    static IMStore* getInstance();

    IMStore();

    void setIMKernel(IMKernel* imKernel);

    IMKernel* getIMKernel();

    void setMainWidget(MainWidget* mainWidget);

    MainWidget* getMainWidget();

    ChatDialog* getChatDialog();

    void openChatPage(int id);

    void closeChatPage(int id);

    bool isOpenChatPage(int id);

    void setFriendWidget(FriendWidget* friendWidget);

    void setSelf(const QJsonObject& json);

    void addFriends(const QJsonObject& json);

    FGSWidget* getFGSWidget();

private:
    Self* m_self;
    IMKernel* m_imKernel = nullptr;
    MainWidget* m_mainWidget = nullptr;
    ChatDialog* m_chatDialog = nullptr;
    QSet<int> m_openChatPageIds;
    FriendWidget* m_friendWidget = nullptr;
    QMap<int, Friend*> m_friends;
    QMap<int, FriendItem*> m_friendItems;
    FGSWidget* m_fgsWidget = nullptr;
};

#endif
