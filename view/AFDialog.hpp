#ifndef view_AFDialog_hpp
#define view_AFDialog_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class AFDialog;
}
QT_END_NAMESPACE

class AFDialog : public QWidget {
    Q_OBJECT

public:
    explicit AFDialog(QWidget* parent = nullptr);

    ~AFDialog() override;

    void setId(int id);

    int getId() const;

    void setAvatar(const QString& url);

    QString getAvatar();

    void setNickname(const QString& nickname);

    QString getNickname();

private slots:
    void addFriend();

private:
    Ui::AFDialog* ui;
    int m_id{};
    QString m_avatarUrl;
    QString m_nickname;
};

#endif
