#ifndef view_FSRWidget_hpp
#define view_FSRWidget_hpp

#include <QWidget>

#include "view/AFDialog.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class FSRItem;
}
QT_END_NAMESPACE

class FSRItem : public QWidget {
    Q_OBJECT

public:
    explicit FSRItem(QWidget* parent = nullptr);

    ~FSRItem() override;

    void setId(int id);

    [[nodiscard]] int getId() const;

    void setAvatar(const QString& url);

    QString getAvatar();

    void setNickname(const QString& nickname);

    QString getNickname();

private:
    Ui::FSRItem* ui;
    int m_id{};
    QString m_avatarUrl;
    QString m_nickname;
    AFDialog* m_dialog = nullptr;
};

#endif
