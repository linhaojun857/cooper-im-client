#ifndef entity_Entity_hpp
#define entity_Entity_hpp

#include <QJsonArray>
#include <QJsonObject>
#include <QString>

struct SyncState {
    int server_state_err{};
    int friend_sync_state{};
    int person_message_sync_state{};
    QVector<QPair<int, int>> friendIds;
    QVector<QPair<int, int>> personMessageIds;

    SyncState() = default;

    explicit SyncState(int friend_sync_state, int person_sync_person_message, const QVector<QPair<int, int>>& friendIds,
                       const QVector<QPair<int, int>>& personMessageIds) {
        this->friend_sync_state = friend_sync_state;
        this->person_message_sync_state = person_sync_person_message;
        this->friendIds = friendIds;
        this->personMessageIds = personMessageIds;
    }

    static SyncState fromJson(const QJsonObject& json) {
        int friend_sync_state = json["friend_sync_state"].toInt();
        int person_message_sync_state = json["person_message_sync_state"].toInt();
        QVector<QPair<int, int>> friendIds_temp;
        QVector<QPair<int, int>> personMessageIds_temp;
        QJsonArray friendIds_json = json["friendIds"].toArray();
        QJsonArray personMessageIds_json = json["personMessageIds"].toArray();
        for (const auto& f : friendIds_json) {
            auto arr = f.toArray();
            friendIds_temp.push_back(QPair<int, int>(arr[0].toInt(), arr[1].toInt()));
        }
        for (const auto& p : personMessageIds_json) {
            auto arr = p.toArray();
            personMessageIds_temp.push_back(QPair<int, int>(arr[0].toInt(), arr[1].toInt()));
        }
        SyncState state(friend_sync_state, person_message_sync_state, friendIds_temp, personMessageIds_temp);
        return state;
    }
};

struct SyncRecord {
    int id;
    long timestamp;

    static const QString createTableSql;
};

struct Self {
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

struct Friend {
    int id{};
    QString username;
    QString nickname;
    QString avatar;
    QString status;
    QString feeling;
    QString session_id;

    static const QString createTableSql;

    Friend() = default;

    Friend(int id, const QString& username, const QString& nickname, const QString& avatar, const QString& status,
           const QString& feeling, const QString& session_id) {
        this->id = id;
        this->username = username;
        this->nickname = nickname;
        this->avatar = avatar;
        this->status = status;
        this->feeling = feeling;
        this->session_id = session_id;
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

    Friend(const Friend& fri) {
        this->id = fri.id;
        this->username = fri.username;
        this->nickname = fri.nickname;
        this->avatar = fri.avatar;
        this->status = fri.status;
        this->feeling = fri.feeling;
    }

    static Friend* fromJson(const QJsonObject& json) {
        auto f = new Friend(json["id"].toInt(), json["username"].toString(), json["nickname"].toString(),
                            json["avatar"].toString(), json["status"].toString(), json["feeling"].toString(),
                            json["session_id"].toString());
        return f;
    }

    static Friend fromJsonCommon(const QJsonObject& json) {
        Friend fri(json["id"].toInt(), json["username"].toString(), json["nickname"].toString(),
                   json["avatar"].toString(), json["status"].toString(), json["feeling"].toString(),
                   json["session_id"].toString());
        return fri;
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

struct GroupApply {
    int id{};
    int from_id{};
    int to_id{};
    int group_id{};
    QString from_avatar;
    QString from_nickname;
    QString to_avatar;
    QString to_name;
    QString reason;
    // 0: 待处理 1: 通过申请 2: 拒绝申请
    int agree{};

    GroupApply() = default;

    GroupApply(int id, int from_id, int to_id, int group_id, const QString& from_avatar, const QString& from_nickname,
               const QString& to_avatar, const QString& to_name, const QString& reason, int agree) {
        this->id = id;
        this->from_id = from_id;
        this->to_id = to_id;
        this->group_id = group_id;
        this->from_avatar = from_avatar;
        this->from_nickname = from_nickname;
        this->to_avatar = to_avatar;
        this->to_name = to_name;
        this->reason = reason;
        this->agree = agree;
    }

    static GroupApply* fromJson(const QJsonObject& json) {
        auto ga = new GroupApply(json["id"].toInt(), json["from_id"].toInt(), json["to_id"].toInt(),
                                 json["group_id"].toInt(), json["from_avatar"].toString(),
                                 json["from_nickname"].toString(), json["to_avatar"].toString(),
                                 json["to_name"].toString(), json["reason"].toString(), json["agree"].toInt());
        return ga;
    }
};

struct PersonMessage {
    int id{};
    int from_id{};
    int to_id{};
    QString session_id;
    int message_type{};
    QString message;
    QString file_url;
    time_t timestamp{};

    static const QString createTableSql;

    PersonMessage() = default;

    PersonMessage(int id, int from_id, int to_id, const QString& session_id, int message_type, const QString& message,
                  const QString& file_url, time_t timestamp) {
        this->id = id;
        this->from_id = from_id;
        this->to_id = to_id;
        this->session_id = session_id;
        this->message_type = message_type;
        this->message = message;
        this->file_url = file_url;
        this->timestamp = timestamp;
    }

    [[nodiscard]] QJsonObject toJson() const {
        QJsonObject json;
        json["id"] = id;
        json["from_id"] = from_id;
        json["to_id"] = to_id;
        json["session_id"] = session_id;
        json["message_type"] = message_type;
        json["message"] = message;
        json["file_url"] = file_url;
        json["timestamp"] = timestamp;
        return json;
    }

    static PersonMessage fromJson(const QJsonObject& json) {
        PersonMessage pm(json["id"].toInt(), json["from_id"].toInt(), json["to_id"].toInt(),
                         json["session_id"].toString(), json["message_type"].toInt(), json["message"].toString(),
                         json["file_url"].toString(), json["timestamp"].toInt());
        return pm;
    }
};

#endif
