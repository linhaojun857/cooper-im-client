#include "entity/Entity.hpp"

const QString SyncRecord::createTableSql =
    "create table if not exists sync_record (\n"
    "    id integer primary key autoincrement,\n"
    "    timestamp integer not null\n"
    ");";

const QString Self::createTableSql =
    "create table if not exists self (\n"
    "    id integer primary key autoincrement,\n"
    "    self_id integer unique not null,\n"
    "    username varchar(45) not null,\n"
    "    nickname varchar(45) not null,\n"
    "    avatar varchar(45) not null,\n"
    "    status varchar(45) not null,\n"
    "    feeling varchar(100) not null\n"
    ");";

const QString Friend::createTableSql =
    "create table if not exists friend (\n"
    "    id integer primary key autoincrement,\n"
    "    friend_id integer unique not null,\n"
    "    username varchar(45) not null,\n"
    "    nickname varchar(45) not null,\n"
    "    avatar varchar(45) not null,\n"
    "    status varchar(45) not null,\n"
    "    feeling varchar(100) not null\n"
    ");";

const QString PersonMessage::createTableSql =
    "create table if not exists person_message (\n"
    "    id integer primary key autoincrement,\n"
    "    msg_id integer unique not null,\n"
    "    from_id integer not null,\n"
    "    to_id integer not null,\n"
    "    content varchar(100) not null,\n"
    "    type integer not null,\n"
    "    timestamp integer not null,\n"
    "    status integer not null\n"
    ");";
