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

#endif
