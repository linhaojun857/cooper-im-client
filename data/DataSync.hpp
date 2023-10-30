#ifndef data_DataSync_hpp
#define data_DataSync_hpp

#include "entity/Entity.hpp"

class DataSync {
public:
    static bool isFirstSync();

    static SyncState getSyncState();

    static void syncAll();

    static void syncFriends(bool isFirstSync, SyncState* syncState);

    static void syncFriendsByServerPush(const QJsonObject& json);

    static void syncPersonMessages(bool isFirstSync, SyncState* syncState);

    static void syncPersonMessagesBuServerPush(const QJsonObject& json);
};

#endif
