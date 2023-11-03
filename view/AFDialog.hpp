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

    [[nodiscard]] int getId() const;

    void setMode(int mode);

    void setAvatar(const QString& url);

    QString getAvatar();

    void setName(const QString& name);

    QString getName();

private slots:
    void addFriend();

private:
    Ui::AFDialog* ui;
    int m_id = 0;
    // 0: friend 1: group
    int m_mode{};
    QString m_avatarUrl;
    QString m_name;
};

#endif
