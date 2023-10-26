#include "DataSync.hpp"

#include <QJsonArray>

#include "define/IMDefine.hpp"
#include "entity/Entity.hpp"
#include "store/IMStore.hpp"
#include "util/HttpUtil.hpp"

bool DataSync::isFirstSync() {
    qDebug() << "DataSync::isFirstSync()";
    QSqlQuery query(*IMStore::getInstance()->getDatabase());
    query.exec("select * from sync_record");
    return !query.next();
}

SyncState DataSync::getSyncState() {
    qDebug() << "DataSync::getSyncState()";
    QJsonObject json;
    SyncState state{};
    json["token"] = IMStore::getInstance()->getToken();
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/getSyncState", json);
    if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
        qDebug() << "get sync state failed! error: " << ret["msg"].toString();
        state.server_state = 1;
        return state;
    }
    return SyncState::fromJson(ret);
}

void DataSync::syncAll() {
    qDebug() << "DataSync::syncAll()";
    auto isFirstSync = DataSync::isFirstSync();
    auto syncState = DataSync::getSyncState();
    DataSync::syncFriends(isFirstSync, syncState);
    QSqlQuery query(*IMStore::getInstance()->getDatabase());
    query.exec("insert into sync_record (timestamp) values (0)");
}

void DataSync::syncFriends(bool isFirstSync, const SyncState& syncState) {
    qDebug() << "DataSync::syncFriends()";
    QJsonObject json;
    json["token"] = IMStore::getInstance()->getToken();
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/getFriends", json);
    if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
        qDebug() << "sync friends failed! error: " << ret["msg"].toString();
    }
    if (isFirstSync) {
        QJsonArray friends = ret["friends"].toArray();
        for (const auto& f : friends) {
            auto obj = f.toObject();
            auto fri = Friend::fromJson(obj);
            QSqlQuery query(*IMStore::getInstance()->getDatabase());
            QString sql = QString(
                              "insert into friend (friend_id ,username, nickname, avatar, status, feeling) "
                              "values (%1 ,'%2', '%3', '%4', '%5', '%6')")
                              .arg(fri->id)
                              .arg(fri->username, fri->nickname, fri->avatar, fri->status, fri->feeling);
            qDebug() << sql;
            if (!query.exec(sql)) {
                qDebug() << query.lastError();
            } else {
                qDebug() << "inserted";
            }
        }
    }
    if (syncState.server_state == 0 && syncState.friend_sync_state == 1) {
        QSqlQuery query(*IMStore::getInstance()->getDatabase());
        query.exec("select friend_id from friend");
        QSet<int> ids;
        while (query.next()) {
            ids.insert(query.value(0).toInt());
        }
        QJsonArray friends = ret["friends"].toArray();
        for (const auto& f : friends) {
            auto obj = f.toObject();
            auto fri = Friend::fromJson(obj);
            if (!ids.contains(fri->id)) {
                QString sql = QString(
                                  "insert into friend (friend_id ,username, nickname, avatar, status, feeling) "
                                  "values (%1 ,'%2', '%3', '%4', '%5', '%6')")
                                  .arg(fri->id)
                                  .arg(fri->username, fri->nickname, fri->avatar, fri->status, fri->feeling);
                qDebug() << sql;
                if (!query.exec(sql)) {
                    qDebug() << query.lastError();
                } else {
                    qDebug() << "inserted";
                }
            } else if (syncState.updated_friends.contains(fri->id)) {
                QString sql = QString(
                                  "update friend set username = '%1', nickname = '%2', avatar = '%3', status = '%4', "
                                  "feeling = '%5' "
                                  "where friend_id = %6")
                                  .arg(fri->username, fri->nickname, fri->avatar, fri->status, fri->feeling)
                                  .arg(fri->id);
                qDebug() << sql;
                if (!query.exec(sql)) {
                    qDebug() << query.lastError();
                } else {
                    qDebug() << "updated";
                }
            }
        }
    }
}

void DataSync::syncFriendsByServerPush(const QJsonObject& json) {
    auto fri = Friend::fromJson(json);
    QSqlQuery query(*IMStore::getInstance()->getDatabase());
    query.exec(QString("select friend_id from friend where friend_id = %1").arg(fri->id));
    if (query.next()) {
        QString sql = QString(
                          "update friend set username = '%1', nickname = '%2', avatar = '%3', status = '%4', feeling = "
                          "'%5' "
                          "where friend_id = %6")
                          .arg(fri->username, fri->nickname, fri->avatar, fri->status, fri->feeling)
                          .arg(fri->id);
        qDebug() << sql;
        if (!query.exec(sql)) {
            qDebug() << query.lastError();
        } else {
            qDebug() << "updated";
        }
    } else {
        QString sql = QString(
                          "insert into friend (friend_id ,username, nickname, avatar, status, feeling) "
                          "values (%1 ,'%2', '%3', '%4', '%5', '%6')")
                          .arg(fri->id)
                          .arg(fri->username, fri->nickname, fri->avatar, fri->status, fri->feeling);
        qDebug() << sql;
        if (!query.exec(sql)) {
            qDebug() << query.lastError();
        } else {
            qDebug() << "inserted";
        }
    }
}
