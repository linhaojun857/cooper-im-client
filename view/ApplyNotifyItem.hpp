#ifndef view_FANIItem_hpp
#define view_FANIItem_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class ApplyNotifyItem;
}
QT_END_NAMESPACE

class ApplyNotifyItem : public QWidget {
    Q_OBJECT

public:
    explicit ApplyNotifyItem(QWidget* parent = nullptr);

    ~ApplyNotifyItem() override;

    void setIPMode(int mode);

    void setFGMode(int mode);

    void setFromId(int fromId);

    void setAvatar(const QString& url);

    void setName(const QString& nickname);

    void setOperation(const QString& operation);

    void setStatus(const QString& status);

    void setReason(const QString& reason);

private:
    void responseFriendApply(int agree);

private:
    Ui::ApplyNotifyItem* ui;
    // 0: initiative 1: passive
    int m_IPMode = 0;
    // 0: friend 1: group
    int m_FGMode = 0;
    int m_fromId{};
    QString m_avatarUrl;
    QString m_name;
    QString m_reason;
};

#endif
