#include <QDebug>

#include "entity/Entity.hpp"

Friend test() {
    Friend friend1(1, "username1", "nickname1", "avatar1", "status1", "feeling1");
    return friend1;
}

int main() {
    Friend friend2 = test();
    // 打印friend2
    qDebug() << "friend2.id: " << friend2.id;
    qDebug() << "friend2.username: " << friend2.username;
    qDebug() << "friend2.nickname: " << friend2.nickname;
    return 0;
}
