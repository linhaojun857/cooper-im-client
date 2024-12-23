#ifndef view_MessageItem_hpp
#define view_MessageItem_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MessageItem;
}
QT_END_NAMESPACE

class MessageItem : public QWidget {
    Q_OBJECT

public:
    explicit MessageItem(QWidget* parent = nullptr);

    ~MessageItem() override;

    void setMode(int mode);

    void setId(int id);

    [[nodiscard]] int getId() const;

    void setAvatar(const QString& url);

    void setName(const QString& name);

    void setRecentMsg(const QString& recentMsg);

    void setTime(long long timestamp);

    long long getTime() const;

    void debug();

private:
    Ui::MessageItem* ui;
    int m_mode = 0;
    int m_id = 0;
    QString m_avatarUrl;
    QString m_name;
    QString m_recentMsg;
    long long m_timestamp = 0;
    QString m_time;
};

#endif
