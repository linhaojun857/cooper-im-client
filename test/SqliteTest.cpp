#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "entity/Entity.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("im.db");
    }
    if (!database.open()) {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    } else {
        qDebug() << "Succeed to connect database.";
    }
    QSqlQuery query;
    if (query.exec(Self::createTableSql)) {
        qDebug() << "create table self";
    } else {
        qDebug() << query.lastError();
    }
    if (query.exec(Friend::createTableSql)) {
        qDebug() << "create table friend";
    } else {
        qDebug() << query.lastError();
    }
    database.exec(Friend::createTableSql);
    Friend f("username", "nickname", "avatar", "status", "feeling");
    QString sql = QString(
                      "insert into friend (username, nickname, avatar, status, feeling) "
                      "values ('%1', '%2', '%3', '%4', '%5')")
                      .arg(f.username, f.nickname, f.avatar, f.status, f.feeling);
    qDebug() << sql;
    if (!query.exec(sql)) {
        qDebug() << query.lastError();
    } else {
        qDebug() << "inserted";
    }
    database.close();
    return 0;
}
