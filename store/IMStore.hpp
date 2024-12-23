#ifndef store_IMStore_hpp
#define store_IMStore_hpp

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QWebEngineView>

#include "entity/Entity.hpp"
#include "view/CGWidget.hpp"
#include "view/ChatDialog.hpp"
#include "view/FGSWidget.hpp"
#include "view/FriendItem.hpp"
#include "view/FriendWidget.hpp"
#include "view/LiveLobbyWidget.hpp"
#include "view/LivePlayerDialog.hpp"
#include "view/LiveRecordDialog.hpp"
#include "view/MainWidget.hpp"
#include "view/MessageWidget.hpp"
#include "view/NotifyWidget.hpp"
#include "view/PyqWidget.hpp"

class IMKernel;

class IMStore {
public:
    static IMStore* getInstance();

    QSqlDatabase* getDatabase();

    IMStore();

    ~IMStore();

    void setIMKernel(IMKernel* imKernel);

    IMKernel* getIMKernel();

    void setMainWidget(MainWidget* mainWidget);

    MainWidget* getMainWidget();

    ChatDialog* getChatDialog();

    void openPersonChatPage(int id);

    void closePersonChatPage(int id);

    bool isOpenPersonChatPage(int id);

    void openGroupChatPage(int id);

    void closeGroupChatPage(int id);

    bool isOpenGroupChatPage(int id);

    void setFriendWidget(FriendWidget* friendWidget);

    void setGroupWidget(GroupWidget* groupWidget);

    void setMessageWidget(MessageWidget* messageWidget);

    void setNotifyWidget(NotifyWidget* notifyWidget);

    NotifyWidget* getNotifyWidget();

    void setSelf(const QJsonObject& json);

    Self* getSelf();

    void setMainWidgetInfo();

    void setToken(const QJsonObject& json);

    void setToken(const QString& token);

    QString getToken();

    void addFriend(const QJsonObject& json);

    void addGroup(const QJsonObject& json);

    [[nodiscard]] FGSWidget* getFGSWidget() const;

    [[nodiscard]] CGWidget* getCGWidget() const;

    void addFSRs(const QJsonObject& json);

    void addGSRs(const QJsonObject& json);

    void addFriendApplyI(FriendApply* friendApply);

    void addFANItemI(int id, ApplyNotifyItem* fanItem);

    bool haveFANItemI(int id);

    ApplyNotifyItem* getFANItemI(int id);

    void addFriendApplyP(FriendApply* friendApply);

    void addFANItemP(int id, ApplyNotifyItem* fanItem);

    bool haveFANItemP(int id);

    ApplyNotifyItem* getFANItemP(int id);

    void addGANItemI(int id, ApplyNotifyItem* ganItem);

    bool haveGANItemI(int id);

    ApplyNotifyItem* getGANItemI(int id);

    void addGANItemP(int id, ApplyNotifyItem* ganItem);

    bool haveGANItemP(int id);

    ApplyNotifyItem* getGANItemP(int id);

    Friend* getFriend(int id);

    Group* getGroup(int id);

    void loadWidget();

    void loadFriendWidget();

    void loadGroupWidget();

    void loadMessageWidget();

    void loadPersonMessageWidget();

    void loadGroupMessageWidget();

    void updatePersonMessageItem(const PersonMessage& pm);

    void movePersonMessageItemToTop(int userId);

    void updateGroupMessageItem(const GroupMessage& gm);

    void moveGroupMessageItemToTop(int groupId);

    QString getLatestPersonMessageByUserId(int userId);

    QString getLatestGroupMessageByGroupId(int groupId);

    static LiveLobbyWidget* getLiveLobbyWidget();

    static PyqWidget* getPyqWidget();

    static LiveRecordDialog* getLiveRecordDialog();

    static LivePlayerDialog* getLivePlayerDialog();

    void setVideoCallDialog(VideoCallDialog* videoCallDialog);

    VideoCallDialog* getVideoCallDialog();

private:
    QSqlDatabase m_database;
    Self* m_self = nullptr;
    QString m_token;
    IMKernel* m_imKernel = nullptr;
    MainWidget* m_mainWidget = nullptr;
    ChatDialog* m_chatDialog = nullptr;
    QSet<int> m_openPersonChatPageIds;
    QSet<int> m_openGroupChatPageIds;
    FriendWidget* m_friendWidget = nullptr;
    GroupWidget* m_groupWidget = nullptr;
    MessageWidget* m_messageWidget = nullptr;
    QMap<int, Friend*> m_friends;
    QMap<int, FriendItem*> m_friendItems;
    QMap<int, MessageItem*> m_personMessageItems;
    QMap<int, MessageItem*> m_groupMessageItems;
    FGSWidget* m_fgsWidget = nullptr;
    CGWidget* m_cgWidget = nullptr;
    QVector<FriendApply*> m_friendApplyIs;
    QMap<int, ApplyNotifyItem*> m_fanItemIs;
    QVector<FriendApply*> m_friendApplyPs;
    QMap<int, ApplyNotifyItem*> m_fanItemPs;
    QMap<int, ApplyNotifyItem*> m_ganItemIs;
    QMap<int, ApplyNotifyItem*> m_ganItemPs;
    NotifyWidget* m_notifyWidget = nullptr;
    QMap<int, Group*> m_groups;
    QMap<int, GroupItem*> m_groupItems;
    VideoCallDialog* m_videoCallDialog = nullptr;
};

#endif
