#ifndef view_FriendItem_hpp
#define view_FriendItem_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class FriendItem;
}
QT_END_NAMESPACE

class FriendItem : public QWidget {
    Q_OBJECT

public:
    explicit FriendItem(QWidget* parent = nullptr);

    ~FriendItem() override;

    void setId(int id);

    void setAvatar(const QString& url);

    void setNickName(const QString& nickname);

    void setStatusAndFeeling(const QString& status, const QString& feeling);

private:
    Ui::FriendItem* ui;
    int m_id = 0;
    QString m_avatarUrl;
    QString m_nickName;
    QString m_status;
    QString m_feeling;
};

#endif
