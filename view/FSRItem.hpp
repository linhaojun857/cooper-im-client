#ifndef view_FSRWidget_hpp
#define view_FSRWidget_hpp

#include <QWidget>

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

    void setAvatar(const QString& url);

    QString getAvatar();

    void setNickname(const QString& nickname);

    QString getNickname();

private:
    Ui::FSRItem* ui;
    QString m_avatarUrl;
    QString m_nickname;
};

#endif
