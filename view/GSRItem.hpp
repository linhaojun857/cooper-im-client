#ifndef view_GSRItem_hpp
#define view_GSRItem_hpp

#include <QWidget>
#include <view/AFDialog.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class GSRItem;
}
QT_END_NAMESPACE

class GSRItem : public QWidget {
    Q_OBJECT

public:
    explicit GSRItem(QWidget* parent = nullptr);

    ~GSRItem() override;

    void setId(int id);

    void setAvatar(const QString& url);

    void setName(const QString& name);

    void setMemberCount(int memberCount);

private:
    Ui::GSRItem* ui;
    int m_id = 0;
    QString m_avatarUrl;
    QString m_name;
    AFDialog* m_dialog = nullptr;
};

#endif
