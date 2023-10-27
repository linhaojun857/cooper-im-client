#include "DataSync.hpp"

#include <QJsonArray>

#include "core/IMKernel.hpp"
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
        state.server_state_err = 1;
        return state;
    }
    return SyncState::fromJson(ret);
}

void DataSync::syncAll() {
    qDebug() << "DataSync::syncAll()";
    auto isFirstSync = DataSync::isFirstSync();
    auto syncState = DataSync::getSyncState();
    DataSync::syncFriends(isFirstSync, &syncState);
    QSqlQuery query(*IMStore::getInstance()->getDatabase());
    query.exec(QString("insert into sync_record (timestamp) values (%1)").arg(time(nullptr)));
    IMStore::getInstance()->getIMKernel()->sendSyncCompleteMsg();
}

void DataSync::syncFriends(bool isFirstSync, SyncState* syncState) {
    qDebug() << "DataSync::syncFriends()";
    SyncState state{};
    if (!syncState) {
        QJsonObject json;
        json["token"] = IMStore::getInstance()->getToken();
        auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/getSyncState", json);
        if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
            qDebug() << "get sync state failed! error: " << ret["msg"].toString();
            state.server_state_err = 1;
        } else {
            state = SyncState::fromJson(ret);
        }
        syncState = &state;
    }
    IMStore::getInstance()->getDatabase()->transaction();
    QSqlQuery query(*IMStore::getInstance()->getDatabase());
    try {
        if (isFirstSync) {
            if (!query.exec("delete from friend")) {
                qDebug() << query.lastError();
                throw std::exception();
            } else {
                qDebug() << "deleted";
            }
            QJsonObject json;
            json["token"] = IMStore::getInstance()->getToken();
            auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/getFriends", json);
            if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
                qDebug() << "sync friends failed! error: " << ret["msg"].toString();
            }
            QJsonArray friends = ret["friends"].toArray();
            for (const auto& f : friends) {
                auto obj = f.toObject();
                auto fri = Friend::fromJsonCommon(obj);
                QString sql = QString(
                                  "insert into friend (friend_id ,username, nickname, avatar, status, feeling) "
                                  "values (%1 ,'%2', '%3', '%4', '%5', '%6')")
                                  .arg(fri.id)
                                  .arg(fri.username, fri.nickname, fri.avatar, fri.status, fri.feeling);
                qDebug() << sql;
                if (!query.exec(sql)) {
                    qDebug() << query.lastError();
                    throw std::exception();
                } else {
                    qDebug() << "inserted";
                }
            }
        } else if (syncState->server_state_err == 0 && syncState->friend_sync_state == 1) {
            QJsonObject json;
            json["token"] = IMStore::getInstance()->getToken();
            QJsonArray friendIdsJson;
            for (const auto& item : syncState->updated_friendIds) {
                friendIdsJson.push_back(item.first);
            }
            json["friends"] = friendIdsJson;
            auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/getFriendsByIds", json);
            if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
                qDebug() << "sync friends failed! error: " << ret["msg"].toString();
            }
            QJsonArray friends = ret["friends"].toArray();
            query.exec("select friend_id from friend");
            QMap<int, Friend> friendMap;
            for (const auto& f : friends) {
                auto obj = f.toObject();
                auto fri = Friend::fromJsonCommon(json);
                friendMap[fri.id] = fri;
            }
            for (const auto& updateId : syncState->updated_friendIds) {
                if (updateId.second == SYNC_DATA_FRIEND_ENTITY_INSERT) {
                    auto fri = friendMap[updateId.first];
                    QString sql = QString(
                                      "insert into friend (friend_id ,username, nickname, avatar, status, feeling) "
                                      "values (%1 ,'%2', '%3', '%4', '%5', '%6')")
                                      .arg(fri.id)
                                      .arg(fri.username, fri.nickname, fri.avatar, fri.status, fri.feeling);
                    qDebug() << sql;
                    if (!query.exec(sql)) {
                        qDebug() << query.lastError();
                        throw std::exception();
                    } else {
                        qDebug() << "inserted";
                    }
                } else if (updateId.second == SYNC_DATA_FRIEND_ENTITY_UPDATE) {
                    auto fri = friendMap[updateId.first];
                    QString sql =
                        QString(
                            "update friend set username = '%1', nickname = '%2', avatar = '%3', status = '%4', "
                            "feeling = '%5' "
                            "where friend_id = %6")
                            .arg(fri.username, fri.nickname, fri.avatar, fri.status, fri.feeling)
                            .arg(fri.id);
                    qDebug() << sql;
                    if (!query.exec(sql)) {
                        qDebug() << query.lastError();
                        throw std::exception();
                    } else {
                        qDebug() << "updated";
                    }
                } else if (updateId.second == SYNC_DATA_FRIEND_ENTITY_DELETE) {
                    QString sql = QString("delete from friend where friend_id = %1").arg(updateId.first);
                    qDebug() << sql;
                    if (!query.exec(sql)) {
                        qDebug() << query.lastError();
                        throw std::exception();
                    } else {
                        qDebug() << "deleted";
                    }
                }
            }
        }
    } catch (const std::exception& ex) {
        IMStore::getInstance()->getDatabase()->rollback();
    }
    IMStore::getInstance()->getDatabase()->commit();
}

void DataSync::syncFriendsByServerPush(const QJsonObject& json) {
    IMStore::getInstance()->getDatabase()->transaction();
    try {
        QSqlQuery query(*IMStore::getInstance()->getDatabase());
        if (json["status"].toInt() == SYNC_DATA_FRIEND_ENTITY_INSERT) {
            auto fri = Friend::fromJsonCommon(json);
            QString sql = QString(
                              "insert into friend (friend_id ,username, nickname, avatar, status, feeling) "
                              "values (%1 ,'%2', '%3', '%4', '%5', '%6')")
                              .arg(fri.id)
                              .arg(fri.username, fri.nickname, fri.avatar, fri.status, fri.feeling);
            qDebug() << sql;
            if (!query.exec(sql)) {
                qDebug() << query.lastError();
                throw std::exception();
            } else {
                qDebug() << "inserted";
            }
        } else if (json["status"].toInt() == SYNC_DATA_FRIEND_ENTITY_UPDATE) {
            auto fri = Friend::fromJsonCommon(json);
            QString sql =
                QString(
                    "update friend set username = '%1', nickname = '%2', avatar = '%3', status = '%4', feeling = "
                    "'%5' "
                    "where friend_id = %6")
                    .arg(fri.username, fri.nickname, fri.avatar, fri.status, fri.feeling)
                    .arg(fri.id);
            qDebug() << sql;
            if (!query.exec(sql)) {
                qDebug() << query.lastError();
                throw std::exception();
            } else {
                qDebug() << "updated";
            }
        } else if (json["status"].toInt() == SYNC_DATA_FRIEND_ENTITY_DELETE) {
            int userId = json["user_id"].toInt();
            QString sql = QString("delete from friend where friend_id = %1").arg(userId);
            qDebug() << sql;
            if (!query.exec(sql)) {
                qDebug() << query.lastError();
                throw std::exception();
            } else {
                qDebug() << "deleted";
            }
        }
    } catch (const std::exception& ex) {
        IMStore::getInstance()->getDatabase()->rollback();
    }
    IMStore::getInstance()->getDatabase()->commit();
}