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

    int getId() const;

    void setAvatar(const QString& url);

    QString getAvatar();

    void setName(const QString& name);

    QString getName();

private:
    Ui::ChatItem* ui;
    int m_id = 0;
    QString m_avatar;
    QString m_name;
};

#endif
