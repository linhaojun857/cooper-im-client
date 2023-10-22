#ifndef view_FANIItem_hpp
#define view_FANIItem_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class FANItem;
}
QT_END_NAMESPACE

// FANI: Friend Apply Notify
class FANItem : public QWidget {
    Q_OBJECT

public:
    explicit FANItem(QWidget* parent = nullptr);

    ~FANItem() override;

    void setMode(int mode);

    void setFromId(int fromId);

    void setAvatar(const QString& url);

    void setNickname(const QString& nickname);

    void setReason(const QString& reason);

    void setStatus(const QString& status);

private:
    void responseFriendApply(int agree);

private:
    Ui::FANItem* ui;
    // 0: initiative 1: passive
    int m_mode = 0;
    int m_fromId{};
    QString m_avatarUrl;
    QString m_nickname;
    QString m_reason;
};

#endif
