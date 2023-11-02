#ifndef view_GSRItem_hpp
#define view_GSRItem_hpp

#include <QWidget>

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
    void handleUserClickAddPushButton();

private:
    Ui::GSRItem* ui;
    int m_id = 0;
};

#endif
