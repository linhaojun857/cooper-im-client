#ifndef view_LiveRoomMsgItem_hpp
#define view_LiveRoomMsgItem_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class LiveRoomMsgItem;
}
QT_END_NAMESPACE

class LiveRoomMsgItem : public QWidget {
    Q_OBJECT

public:
    explicit LiveRoomMsgItem(QWidget* parent = nullptr);

    ~LiveRoomMsgItem() override;

    void setAvatar(const QString& avatarUrl);

    void setNicknameAndMsg(const QString& nickname, const QString& msg);

private:
    Ui::LiveRoomMsgItem* ui;
};

#endif
