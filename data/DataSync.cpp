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
    DataSync::syncPersonMessages(isFirstSync, &syncState);
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
            auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/getAllFriends", json);
            if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
                qDebug() << "sync friends failed! error: " << ret["msg"].toString();
            }
            QJsonArray friends = ret["friends"].toArray();
            for (const auto& f : friends) {
                auto obj = f.toObject();
                auto fri = Friend::fromJsonCommon(obj);
                QString sql =
                    QString(
                        "insert into friend (friend_id ,username, nickname, avatar, status, feeling, session_id) "
                        "values (%1 ,'%2', '%3', '%4', '%5', '%6', '%7')")
                        .arg(fri.id)
                        .arg(fri.username, fri.nickname, fri.avatar, fri.status, fri.feeling, fri.session_id);
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
            auto ret = HttpUtil::post(HTTP_SERVER_URL "/user/getSyncFriends", json);
            if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
                qDebug() << "sync friends failed! error: " << ret["msg"].toString();
            }
            QJsonArray friends = ret["friends"].toArray();
            QMap<int, Friend> friendMap;
            for (const auto& f : friends) {
                auto obj = f.toObject();
                auto fri = Friend::fromJsonCommon(obj);
                friendMap[fri.id] = fri;
            }
            for (const auto& updateId : syncState->friendIds) {
                if (updateId.second == SYNC_DATA_FRIEND_ENTITY_INSERT) {
                    auto fri = friendMap[updateId.first];
                    QString sql =
                        QString(
                            "insert into friend (friend_id ,username, nickname, avatar, status, feeling) "
                            "values (%1 ,'%2', '%3', '%4', '%5', '%6', '%7')")
                            .arg(fri.id)
                            .arg(fri.username, fri.nickname, fri.avatar, fri.status, fri.feeling, fri.session_id);
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
                            "feeling = '%5', session_id = '%6' "
                            "where friend_id = %7")
                            .arg(fri.username, fri.nickname, fri.avatar, fri.status, fri.feeling, fri.session_id)
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
                              "values (%1 ,'%2', '%3', '%4', '%5', '%6', '%7')")
                              .arg(fri.id)
                              .arg(fri.username, fri.nickname, fri.avatar, fri.status, fri.feeling, fri.session_id);
            qDebug() << sql;
            if (!query.exec(sql)) {
                qDebug() << query.lastError();
                throw std::exception();
            } else {
                qDebug() << "inserted";
            }
        } else if (json["status"].toInt() == SYNC_DATA_FRIEND_ENTITY_UPDATE) {
            auto fri = Friend::fromJsonCommon(json);
            QString sql = QString(
                              "update friend set username = '%1', nickname = '%2', avatar = '%3', status = '%4', "
                              "feeling = '%5', session_id = '%6'"
                              "where friend_id = %6")
                              .arg(fri.username, fri.nickname, fri.avatar, fri.status, fri.feeling, fri.session_id)
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

void DataSync::syncPersonMessages(bool isFirstSync, SyncState* syncState) {
    qDebug() << "DataSync::syncPersonMessages()";
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
            if (!query.exec("delete from person_message")) {
                qDebug() << query.lastError();
                throw std::exception();
            } else {
                qDebug() << "deleted";
            }
            QJsonObject json;
            json["token"] = IMStore::getInstance()->getToken();
            auto ret = HttpUtil::post(HTTP_SERVER_URL "/msg/getAllPersonMessages", json);
            if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
                qDebug() << "sync friends failed! error: " << ret["msg"].toString();
            }
            QJsonArray personMessages = ret["personMessages"].toArray();
            for (const auto& pm : personMessages) {
                auto obj = pm.toObject();
                auto personMessage = PersonMessage::fromJson(obj);
                QString sql = QString(
                                  "insert into person_message (msg_id, from_id, to_id, session_id, message_type, "
                                  "message, file_url, timestamp) "
                                  "values (%1, %2, %3, '%4', %5, '%6', '%7', %8)")
                                  .arg(personMessage.id)
                                  .arg(personMessage.from_id)
                                  .arg(personMessage.to_id)
                                  .arg(personMessage.session_id)
                                  .arg(personMessage.message_type)
                                  .arg(personMessage.message, personMessage.file_url)
                                  .arg(personMessage.timestamp);
                qDebug() << sql;
                if (!query.exec(sql)) {
                    qDebug() << query.lastError();
                    throw std::exception();
                } else {
                    qDebug() << "inserted";
                }
            }
        } else if (syncState->server_state_err == 0 && syncState->person_message_sync_state == 1) {
            QJsonObject json;
            json["token"] = IMStore::getInstance()->getToken();
            auto ret = HttpUtil::post(HTTP_SERVER_URL "/msg/getSyncPersonMessages", json);
            if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
                qDebug() << "sync friends failed! error: " << ret["msg"].toString();
            }
            QJsonArray personMessages = ret["personMessages"].toArray();
            QMap<int, PersonMessage> personMessageMap;
            for (const auto& pm : personMessages) {
                auto obj = pm.toObject();
                auto personMessage = PersonMessage::fromJson(obj);
                personMessageMap[personMessage.id] = personMessage;
            }
            for (const auto& updateId : syncState->personMessageIds) {
                if (updateId.second == SYNC_DATA_PERSON_MESSAGE_INSERT) {
                    auto personMessage = personMessageMap[updateId.first];
                    QString sql = QString(
                                      "insert into person_message (msg_id, from_id, to_id, session_id, message_type, "
                                      "message, file_url, timestamp) "
                                      "values (%1, %2, %3, '%4', %5, '%6', '%7', %8)")
                                      .arg(personMessage.id)
                                      .arg(personMessage.from_id)
                                      .arg(personMessage.to_id)
                                      .arg(personMessage.session_id)
                                      .arg(personMessage.message_type)
                                      .arg(personMessage.message, personMessage.file_url)
                                      .arg(personMessage.timestamp);
                    qDebug() << sql;
                    if (!query.exec(sql)) {
                        qDebug() << query.lastError();
                        throw std::exception();
                    } else {
                        qDebug() << "inserted";
                    }
                } else if (updateId.second == SYNC_DATA_PERSON_MESSAGE_DELETE) {
                    QString sql = QString("delete from person_message where msg_id = %1").arg(updateId.first);
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

void DataSync::syncPersonMessagesBuServerPush(const QJsonObject& json) {
    IMStore::getInstance()->getDatabase()->transaction();
    try {
        QSqlQuery query(*IMStore::getInstance()->getDatabase());
        if (json["status"].toInt() == SYNC_DATA_PERSON_MESSAGE_INSERT) {
            auto personMessage = PersonMessage::fromJson(json);
            QString sql = QString(
                              "insert into person_message (msg_id, from_id, to_id, session_id, message_type, "
                              "message, file_url, timestamp) "
                              "values (%1, %2, %3, '%4', %5, '%6', '%7', %8)")
                              .arg(personMessage.id)
                              .arg(personMessage.from_id)
                              .arg(personMessage.to_id)
                              .arg(personMessage.session_id)
                              .arg(personMessage.message_type)
                              .arg(personMessage.message, personMessage.file_url)
                              .arg(personMessage.timestamp);
            qDebug() << sql;
            if (!query.exec(sql)) {
                qDebug() << query.lastError();
                throw std::exception();
            } else {
                qDebug() << "inserted";
            }
        } else if (json["status"].toInt() == SYNC_DATA_PERSON_MESSAGE_DELETE) {
            int msgId = json["msg_id"].toInt();
            QString sql = QString("delete from person_message where msg_id = %1").arg(msgId);
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
