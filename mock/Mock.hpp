#ifndef mock_MockData_hpp
#define mock_MockData_hpp

#include <QVector>

class FriendWidget;
class GroupWidget;
class NotifyWidget;
class FGSWidget;

class Mock {
public:
    static QVector<QString> urls;

    static QVector<QString> nicknames;

    static QVector<QString> statuses;

    static QVector<QString> feelings;

    static QMap<int, QVector<QString>> chatHistory;

    static QVector<QString> addFriendReasons;

    static QVector<QString> groupNames;

    static void InitMockData();

    // FriendWidget Mock Data
    static FriendWidget* friendWidget;

    static void addMockFriendItems();

    // GroupWidget Mock Data
    static GroupWidget* groupWidget;

    static void addMockGroupItems();

    // NotifyWidget Mock Data
    static NotifyWidget* notifyWidget;

    static void addMockNewFriendItems();

    static FGSWidget* fgsWidget;

    static void addMockFGSItems();
};

#endif
