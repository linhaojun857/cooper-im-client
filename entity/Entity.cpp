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

// const QString FriendApply::createTableSql =
//     "create table if not exists friend_apply (\n"
//     "    id integer primary key autoincrement,\n"
//     "    from_id integer not null,\n"
//     "    to_id integer not null,\n"
//     "    from_avatar varchar(45) not null,\n"
//     "    from_nickname varchar(45) not null,\n"
//     "    to_avatar varchar(45) not null,\n"
//     "    to_nickname varchar(45) not null,\n"
//     "    reason varchar(100) not null,\n"
//     "    agree integer not null\n"
//     ");";
