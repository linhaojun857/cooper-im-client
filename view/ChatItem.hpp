#ifndef view_ChatItem_hpp
#define view_ChatItem_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class ChatItem;
}
QT_END_NAMESPACE

class ChatItem : public QWidget {
    Q_OBJECT

public:
    explicit ChatItem(QWidget* parent = nullptr);

    ~ChatItem() override;

    void setId(int id);

    [[nodiscard]] int getId() const;

    void setAvatar(const QString& url);

    QString getAvatar();

    void setName(const QString& name);

    QString getName();

    void setRecentMsg(QString recentMsg);

private:
    Ui::ChatItem* ui;
    int m_id = 0;
    QString m_avatarUrl;
    QString m_name;
    QString m_recentMsg;
};

#endif
