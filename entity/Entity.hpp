#ifndef entity_Entity_hpp
#define entity_Entity_hpp

#include <QString>

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
};

#endif
