#ifndef entity_Entity_hpp
#define entity_Entity_hpp

#include <QJsonArray>
#include <QJsonObject>
#include <QString>

class SyncState {
public:
    int server_state{};
    int friend_sync_state{};
    QVector<int> updated_friends;

    SyncState() = default;

    explicit SyncState(int friend_sync_state, const QVector<int>& updated_friends) {
        this->friend_sync_state = friend_sync_state;
        this->updated_friends = updated_friends;
    }

    static SyncState fromJson(const QJsonObject& json) {
        int server_state = json["server_state"].toInt();
        auto updated_friends = json["updated_friends"].toArray();
        QVector<int> updated_friends_vec;
        for (const auto& f : updated_friends) {
            updated_friends_vec.push_back(f.toInt());
        }
        return SyncState(server_state, updated_friends_vec);
    }
};

class SyncRecord {
public:
    int id;
    long timestamp;

    static const QString createTableSql;
};

class Self {
public:
    int id;
    QString username;
    QString nickname;
    QString avatar;
    QString status;
    QString feeling;

    static const QString createTableSql;

    Self(int id, const QString& username, const QString& nickname, const QString& avatar, const QString& status,
         const QString& feeling) {
        this->id = id;
        this->username = username;
        this->nickname = nickname;
        this->avatar = avatar;
        this->status = status;
        this->feeling = feeling;
    }

    static Self* fromJson(const QJsonObject& json) {
        auto self = new Self(json["id"].toInt(), json["username"].toString(), json["nickname"].toString(),
                             json["avatar"].toString(), json["status"].toString(), json["feeling"].toString());
        return self;
    }
};

class Friend {
public:
    int id;
    QString username;
    QString nickname;
    QString avatar;
    QString status;
    QString feeling;

    static const QString createTableSql;

    Friend(int id, const QString& username, const QString& nickname, const QString& avatar, const QString& status,
           const QString& feeling) {
        this->id = id;
        this->username = username;
        this->nickname = nickname;
        this->avatar = avatar;
        this->status = status;
        this->feeling = feeling;
    }

    Friend(const QString& username, const QString& nickname, const QString& avatar, const QString& status,
           const QString& feeling) {
        this->id = 0;
        this->username = username;
        this->nickname = nickname;
        this->avatar = avatar;
        this->status = status;
        this->feeling = feeling;
    }

    static Friend* fromJson(const QJsonObject& json) {
        auto f = new Friend(json["id"].toInt(), json["username"].toString(), json["nickname"].toString(),
                            json["avatar"].toString(), json["status"].toString(), json["feeling"].toString());
        return f;
    }
};

struct FriendApply {
    int id{};
    int from_id{};
    int to_id{};
    QString from_avatar;
    QString from_nickname;
    QString to_avatar;
    QString to_nickname;
    QString reason;
    // 0: 待处理 1: 通过申请 2: 拒绝申请
    int agree{};

    FriendApply() = default;

    FriendApply(int id, int from_id, int to_id, const QString& from_avatar, const QString& from_nickname,
                const QString& to_avatar, const QString& to_nickname, const QString& reason, int agree) {
        this->id = id;
        this->from_id = from_id;
        this->to_id = to_id;
        this->from_avatar = from_avatar;
        this->from_nickname = from_nickname;
        this->to_avatar = to_avatar;
        this->to_nickname = to_nickname;
        this->reason = reason;
        this->agree = agree;
    }

    static FriendApply* fromJson(const QJsonObject& json) {
        auto fa = new FriendApply(json["id"].toInt(), json["from_id"].toInt(), json["to_id"].toInt(),
                                  json["from_avatar"].toString(), json["from_nickname"].toString(),
                                  json["to_avatar"].toString(), json["to_nickname"].toString(),
                                  json["reason"].toString(), json["agree"].toInt());
        return fa;
    }
};

#endif
