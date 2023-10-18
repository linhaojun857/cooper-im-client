#include "Mock.hpp"

#include <QThread>
#include <QTimer>

#include "view/FriendWidget.hpp"
#include "view/GroupWidget.hpp"
#include "view/NotifyWidget.hpp"

QVector<QString> Mock::urls = {
    "http://thirdqq.qlogo.cn/g?b=oidb&k=jWGicCEAEOGTsXtzplbsBSA&s=40&t=1658928554",
    "http://thirdqq.qlogo.cn/g?b=oidb&k=OgzJGGDUAQWW996sYuIblg&s=40&t=1648976853",
    "http://thirdqq.qlogo.cn/g?b=oidb&k=XkKCoFEue3N8F66EgTRrzQ&s=40&t=1659006735",
    "http://thirdqq.qlogo.cn/g?b=oidb&k=MIgSsjicQfRj7cxIWtd9iaVQ&s=40&t=1657901905",
    "http://thirdqq.qlogo.cn/g?b=oidb&k=msjpp0UlW7UeF2wdODjmZg&s=40&t=1633943518",
    "http://thirdqq.qlogo.cn/g?b=oidb&k=nFHptgpec2khZffJ6tYXHA&s=40&t=1659875078",
    "http://thirdqq.qlogo.cn/g?b=oidb&k=MZGdfr2siaeicUgib1JbicZWbQ&s=40&t=1653931352",
    "http://thirdqq.qlogo.cn/g?b=oidb&k=pia1Uwu0nGicHQYS8rkc4fYQ&s=40&t=1555542524",
    "http://thirdqq.qlogo.cn/g?b=oidb&k=OqTaCibiclZiaV0WjlBs74YVw&s=40&t=1571802774",
    "http://thirdqq.qlogo.cn/g?b=oidb&k=ne4WYYMKm1DkPBzTU6vt6A&s=40&t=1605537409",
    "https://static.linhaojun.top/aurora/avatar/01e049223b03fc230c2b28ae16e9280d.jpg",
    "https://static.linhaojun.top/aurora/avatar/0f4cd4fdabdbd86f1d654fb49a90fafb.jpg",
    "https://static.linhaojun.top/aurora/avatar/281fe50894fdfa0f86f7f9690b526097.png",
    "https://static.linhaojun.top/aurora/avatar/84bfed455789c221619bfe58b0ad2b85.jpeg",
    "https://static.linhaojun.top/aurora/avatar/d7f20623c806e6739cdf030df3610113.jpg",
    "https://static.linhaojun.top/aurora/avatar/c17eca6537e93a2a3f3afc9ebe293589.jpg",
    "https://static.linhaojun.top/aurora/avatar/e608f63258d1a50ab0da6965649c2a37.jpg",
    "https://static.linhaojun.top/aurora/avatar/541e55aaa3752cdcd00367d97ae895ce.jpg",
    "https://static.linhaojun.top/aurora/avatar/f603d1053fb3d40f84e759c27c347a28.jpeg",
    "https://static.linhaojun.top/aurora/avatar/8ac9c63f9d0ba11c7d4d45191c406d18.jpg",
};

QVector<QString> Mock::nicknames = {"开心的火龙果", "二爷",      "你仔细听", "小乖乖",         "忆生i",
                                    "杨酷酷",       "五行缺钱",  "MRBEE",    "星川",           "怀念",
                                    "没毛的小狐狸", "八尺妖剑",  "提露",     "一个超人的角色", "所念皆星河",
                                    "远辰",         "Cold moon", "永恒",     "ZVerify",        "明天一定吃早饭"};

QVector<QString> Mock::statuses = {"忙碌", "在线", "离开", "求锦鲤", "发呆", "胡思乱想"};

QVector<QString> Mock::feelings = {"每一天都是一个新的开始。", "梦想，永不放弃。",
                                   "阳光总在风雨后。",         "不忘初心，方得始终。",
                                   "做自己，因为你独一无二。", "生活不止眼前的苟且，还有诗和远方。",
                                   "幸福是一种心态。",         "努力不一定成功，但不努力必定失败。",
                                   "心若向阳，无谓阴霾。",     "走自己的路，让别人说去吧。",
                                   "生活需要笑容和感激。",     "希望是前进的动力。",
                                   "在黑夜中寻找光明。",       "活在当下，享受每一刻。",
                                   "坚持就是胜利。",           "不怕失败，只怕不再尝试。",
                                   "相信自己，你能做到。",     "勇敢前行，未来在等待。",
                                   "不忙于奔波，不迷失方向。", "活出自己的精彩。"};

QMap<int, QVector<QString>> Mock::chatHistory;

QVector<QString> Mock::groupNames = {"Aurora博客交流群", "SpringBoot交流群", "Java交流群",  "MySQL交流群",
                                     "Linux交流群",      "Redis交流群",      "ES甲流群",    "MyBatis交流群",
                                     "Kafka交流群",      "RabbitMQ交流群",   "测试交流群1", "测试交流群2",
                                     "测试交流群3",      "测试交流群4",      "测试交流群5", "测试交流群6",
                                     "测试交流群7",      "测试交流群8",      "测试交流群9", "测试交流群10"};

QVector<QString> Mock::addFriendReasons{
    "我是你的粉丝，能加个好友吗？", "你好，我是你的同事，能加个好友吗？", "你的开源项目太棒了",
    "你好，我和你一个高中的",       "Hello，Can you add me as a friend?", "I listened to your song, it's great",
};

void Mock::InitMockData() {
    for (int i = 0; i < 10; ++i) {
        chatHistory.insert(i, {});
        for (int j = 0; j < 50; ++j) {
            chatHistory[i].push_back(
                QString("\"%1,这是%2测试聊天记录\"").arg(QString("我是%1").arg(i), QString("第%1条").arg(j)));
        }
    }
}

FriendWidget* Mock::friendWidget = nullptr;

void Mock::addMockFriendItems() {
    for (int i = 0; i < 20; ++i) {
        auto item = new FriendItem();
        item->setId(i);
        item->setAvatar(Mock::urls[i % Mock::urls.size()]);
        item->setNickName(Mock::nicknames[i % Mock::nicknames.size()]);
        item->setStatus(QString("[%1]").arg(Mock::statuses[i % Mock::statuses.size()]));
        item->setFeeling(Mock::feelings[i % Mock::feelings.size()]);
        friendWidget->addFriendItem(item);
    }
}

GroupWidget* Mock::groupWidget = nullptr;

void Mock::addMockGroupItems() {
    for (int i = 0; i < 20; ++i) {
        auto item = new GroupItem();
        item->setId(i);
        item->setAvatar(Mock::urls[i % Mock::urls.size()]);
        item->setGroupName(Mock::groupNames[i % Mock::groupNames.size()]);
        groupWidget->addGroupItem(item);
    }
}

NotifyWidget* Mock::notifyWidget = nullptr;

void Mock::addMockNewFriendItems() {
    for (int i = 0; i < 20; ++i) {
        auto item = new NewFriendItem();
        item->setAvatar(Mock::urls[i % Mock::urls.size()]);
        item->setNickname(Mock::nicknames[i % Mock::nicknames.size()]);
        item->setReason(Mock::addFriendReasons[i % Mock::addFriendReasons.size()]);
        notifyWidget->addNewFriendItem(item);
    }
}
