#ifndef entity_Entity_hpp
#define entity_Entity_hpp

#include <QJsonObject>
#include <QString>

class Self {
public:
    int id;
    QString username;
    QString nickname;
    QString avatar;
    QString status;
    QString feeling;

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

    Friend(int id, const QString& username, const QString& nickname, const QString& avatar, const QString& status,
           const QString& feeling) {
        this->id = id;
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
