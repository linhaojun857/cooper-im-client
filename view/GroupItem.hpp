#ifndef COOPER_IM_CLIENT_VIEW_GROUPITEM_HPP_
#define COOPER_IM_CLIENT_VIEW_GROUPITEM_HPP_

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class GroupItem;
}
QT_END_NAMESPACE

class GroupItem : public QWidget {
    Q_OBJECT

public:
    explicit GroupItem(QWidget* parent = nullptr);

    ~GroupItem() override;

    void setId(int id);

    int getId();

    void setAvatar(const QString& url);

    QString getAvatar();

    void setGroupName(const QString& groupName);

    QString getGroupName();

private:
    Ui::GroupItem* ui;
    int m_id = 0;
    QString m_avatarUrl;
    QString m_groupName;
};

#endif
