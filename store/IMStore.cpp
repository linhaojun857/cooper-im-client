#include "IMStore.hpp"

#include <QJsonArray>
#include <QJsonObject>
#include <QThread>

IMStore* IMStore::getInstance() {
    static IMStore store;
    return &store;
}

QSqlDatabase* IMStore::getDatabase() {
    return &m_database;
}

IMStore::IMStore() {
    m_chatDialog = new ChatDialog();
    m_fgsWidget = new FGSWidget();
    m_cgWidget = new CGWidget();
}

void IMStore::setIMKernel(IMKernel* imKernel) {
    m_imKernel = imKernel;
}

IMKernel* IMStore::getIMKernel() {
    return m_imKernel;
}

void IMStore::setMainWidget(MainWidget* mainWidget) {
    m_mainWidget = mainWidget;
}

MainWidget* IMStore::getMainWidget() {
    return m_mainWidget;
}

ChatDialog* IMStore::getChatDialog() {
    return m_chatDialog;
}

void IMStore::openChatPage(int id) {
    m_openChatPageIds.insert(id);
}

void IMStore::closeChatPage(int id) {
    m_openChatPageIds.remove(id);
}

bool IMStore::isOpenChatPage(int id) {
    return m_openChatPageIds.contains(id);
}

void IMStore::setFriendWidget(FriendWidget* friendWidget) {
    m_friendWidget = friendWidget;
}

void IMStore::setMessageWidget(MessageWidget* messageWidget) {
    m_messageWidget = messageWidget;
}

void IMStore::setNotifyWidget(NotifyWidget* notifyWidget) {
    m_notifyWidget = notifyWidget;
}

NotifyWidget* IMStore::getNotifyWidget() {
    return m_notifyWidget;
}

void IMStore::setSelf(const QJsonObject& json) {
    qDebug() << "IMStore::setSelf";
    m_self = Self::fromJson(json["user"].toObject());
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        m_database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        m_database = QSqlDatabase::addDatabase("QSQLITE");
        m_database.setDatabaseName(QString("%1.db").arg(m_self->username));
    }
    if (!m_database.open()) {
        qDebug() << "Error: Failed to connect database." << m_database.lastError();
        exit(-1);
    } else {
        qDebug() << "Succeed to connect database.";
    }
    m_database.transaction();
    QSqlQuery query(m_database);
    if (!query.exec(Self::createTableSql) || !query.exec(Friend::createTableSql) ||
        !query.exec(SyncRecord::createTableSql) || !query.exec(PersonMessage::createTableSql)) {
        qDebug() << "local data table create failed";
        m_database.rollback();
        exit(-1);
    }
    m_database.commit();
    query.exec("select * from t_self");
    if (!query.next() || query.value(1).toInt() != m_self->id) {
        query.exec("delete from t_sync_record");
    }
    query.exec("delete from t_self");
    QString sql = QString(
                      "insert into t_self (self_id, username, nickname, avatar, status, feeling) "
                      "values (%1, '%2', '%3', '%4', '%5', '%6')")
                      .arg(m_self->id)
                      .arg(m_self->username, m_self->nickname, m_self->avatar, m_self->status, m_self->feeling);
    qDebug() << sql;
    if (!query.exec(sql)) {
        qDebug() << query.lastError();
    } else {
        qDebug() << "inserted";
    }
}

Self* IMStore::getSelf() {
    return m_self;
}

void IMStore::setMainWidgetInfo() {
    m_mainWidget->setAvatar(m_self->avatar);
    m_mainWidget->setNickname(m_self->nickname);
    m_mainWidget->setStatusAndFeeling(m_self->status, m_self->feeling);
}

void IMStore::setToken(const QJsonObject& json) {
    qDebug() << "IMStore::setToken";
    m_token = json["token"].toString();
}

QString IMStore::getToken() {
    return m_token;
}

void IMStore::addFriend(const QJsonObject& json) {
    qDebug() << "IMStore::addFriend";
    auto fri = Friend::fromJson(json);
    auto friendItem = new FriendItem();
    friendItem->setId(fri->id);
    friendItem->setNickName(fri->nickname);
    friendItem->setAvatar(fri->avatar);
    friendItem->setStatusAndFeeling(fri->status, fri->feeling);
    m_friends[fri->id] = fri;
    m_friendItems[fri->id] = friendItem;
    m_friendWidget->addFriendItem(friendItem);
}

FGSWidget* IMStore::getFGSWidget() const {
    return m_fgsWidget;
}

CGWidget* IMStore::getCGWidget() const {
    return m_cgWidget;
}

void IMStore::addFSRs(const QJsonObject& json) {
    qDebug() << "IMStore::addFSRs";
    QJsonArray fsrs = json["fsrs"].toArray();
    int row;
    int column;
    for (int i = 0; i < fsrs.size(); ++i) {
        row = i / 3;
        column = i % 3;
        auto fsrItem = new FSRItem();
        auto obj = fsrs[i].toObject();
        fsrItem->setId(obj["id"].toInt());
        fsrItem->setAvatar(obj["avatar"].toString());
        fsrItem->setNickname(obj["nickname"].toString());
        if (m_fgsWidget) {
            m_fgsWidget->addFSRItem(fsrItem, row, column);
        }
    }
}

void IMStore::addGSRs(const QJsonObject& json) {
    qDebug() << "IMStore::addGSRs";
    QJsonArray gsrs = json["gsrs"].toArray();
    int row;
    int column;
    for (int i = 0; i < gsrs.size(); ++i) {
        row = i / 3;
        column = i % 3;
        auto gsrItem = new GSRItem();
        auto obj = gsrs[i].toObject();
        gsrItem->setId(obj["id"].toInt());
        gsrItem->setAvatar(obj["avatar"].toString());
        gsrItem->setName(obj["name"].toString());
        gsrItem->setMemberCount(obj["member_count"].toInt());
        if (m_fgsWidget) {
            m_fgsWidget->addGSRItem(gsrItem, row, column);
        }
    }
}

void IMStore::addFriendApplyI(FriendApply* friendApply) {
    m_friendApplyIs.append(friendApply);
}

void IMStore::addFANItemI(int id, ApplyNotifyItem* fanItem) {
    m_fanItemIs[id] = fanItem;
}

bool IMStore::haveFANItemI(int id) {
    return m_fanItemIs.contains(id);
}

ApplyNotifyItem* IMStore::getFANItemI(int id) {
    return m_fanItemIs[id];
}

void IMStore::addFriendApplyP(FriendApply* friendApply) {
    m_friendApplyPs.append(friendApply);
}

void IMStore::addFANItemP(int id, ApplyNotifyItem* fanItem) {
    m_fanItemPs[id] = fanItem;
}

bool IMStore::haveFANItemP(int id) {
    return m_fanItemPs.contains(id);
}

ApplyNotifyItem* IMStore::getFANItemP(int id) {
    return m_fanItemPs[id];
}

void IMStore::addGANItemI(int id, ApplyNotifyItem* ganItem) {
    m_ganItemIs[id] = ganItem;
}

bool IMStore::haveGANItemI(int id) {
    return m_ganItemIs.contains(id);
}

ApplyNotifyItem* IMStore::getGANItemI(int id) {
    return m_ganItemIs[id];
}

void IMStore::addGANItemP(int id, ApplyNotifyItem* ganItem) {
    m_ganItemPs[id] = ganItem;
}

bool IMStore::haveGANItemP(int id) {
    return m_ganItemPs.contains(id);
}

ApplyNotifyItem* IMStore::getGANItemP(int id) {
    return m_ganItemPs[id];
}

Friend* IMStore::getFriend(int id) {
    return m_friends[id];
}

void IMStore::loadWidget() {
    loadFriendWidget();
    loadMessageWidget();
}

void IMStore::loadFriendWidget() {
    QSqlQuery query(m_database);
    query.exec("select friend_id, username, nickname, avatar, status, feeling, session_id from t_friend");
    while (query.next()) {
        Friend fri(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(),
                   query.value(3).toString(), query.value(4).toString(), query.value(5).toString(),
                   query.value(6).toString());
        m_friends[fri.id] = new Friend(fri);
        auto friendItem = new FriendItem();
        friendItem->setId(fri.id);
        friendItem->setSessionId(fri.session_id);
        friendItem->setNickName(fri.nickname);
        friendItem->setAvatar(fri.avatar);
        friendItem->setStatusAndFeeling(fri.status, fri.feeling);
        m_friendItems[fri.id] = friendItem;
        m_friendWidget->addFriendItem(friendItem);
    }
}

void IMStore::loadMessageWidget() {
    QSqlQuery query(m_database);
    QString sql(
        "select t_friend.friend_id,\n"
        "       t_friend.nickname,\n"
        "       t_friend.avatar,\n"
        "       t_person_message.message,\n"
        "       t_person_message.timestamp\n"
        "from t_friend\n"
        "         inner join t_person_message\n"
        "                    on t_friend.session_id = t_person_message.session_id\n"
        "where t_person_message.msg_id in (select max(msg_id)\n"
        "                                from t_person_message\n"
        "                                group by t_person_message.session_id)\n"
        "order by t_person_message.msg_id desc;");
    if (!query.exec(sql)) {
        qDebug() << query.lastError().text();
    }
    while (query.next()) {
        auto messageItem = new MessageItem();
        messageItem->setId(query.value(0).toInt());
        messageItem->setName(query.value(1).toString());
        messageItem->setAvatar(query.value(2).toString());
        messageItem->setRecentMsg(query.value(3).toString());
        messageItem->setTime(query.value(4).toLongLong());
        m_messageWidget->addMessageItem(messageItem);
        m_messageItems[messageItem->getId()] = messageItem;
    }
}

void IMStore::updatePersonMessageItem(const PersonMessage& pm) {
    auto messageItem = m_messageItems[pm.from_id];
    messageItem->setRecentMsg(pm.message);
    messageItem->setTime(pm.timestamp);
    movePersonMessageItemToTop(pm.from_id);
}

void IMStore::movePersonMessageItemToTop(int id) {
    auto messageItem = m_messageItems[id];
    m_messageWidget->moveMessageItemToTop(messageItem);
}

QString IMStore::getLatestPersonMessageByUserId(int userId) {
    QSqlQuery query(m_database);
    QString sql1 = QString("select session_id from t_friend where friend_id = %1").arg(userId);
    if (!query.exec(sql1)) {
        qDebug() << query.lastError().text();
        return "";
    }
    if (!query.next()) {
        return "";
    }
    QString sessionId = query.value(0).toString();
    QString sql2 =
        QString("select message from t_person_message where session_id = '%1' order by id desc limit 1").arg(sessionId);
    if (!query.exec(sql2)) {
        qDebug() << query.lastError().text();
        return "";
    }
    if (query.next()) {
        return query.value(0).toString();
    }
    return "";
}
