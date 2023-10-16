#ifndef mock_MockData_hpp
#define mock_MockData_hpp

#include <QVector>

class FriendWidget;
class NotifyWidget;

class Mock {
public:
    static QVector<QString> urls;

    static QVector<QString> nicknames;

    static QVector<QString> statuses;

    static QVector<QString> feelings;

    static QMap<int, QVector<QString>> chatHistory;

    static QVector<QString> addFriendReasons;

    static void InitMockData();

    // FriendWidget Mock Data
    static FriendWidget* friendWidget;

    static void addMockFriendItems();

    // NotifyWidget Mock Data
    static NotifyWidget* notifyWidget;

    static void addMockNewFriendItems();
};

#endif
