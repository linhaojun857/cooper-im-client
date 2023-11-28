#ifndef view_MainWidget_HPP
#define view_MainWidget_HPP

#include <QPushButton>
#include <QWidget>

#include "view/CotPWidget.hpp"
#include "view/MessageWidget.hpp"

class IMKernel;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent = nullptr);

    ~MainWidget() override;

    void setAvatar(const QString& url);

    void setNickname(const QString& nickname);

    void setStatusAndFeeling(const QString& status, const QString& feeling);

private:
    Ui::MainWidget* ui;
    QString m_avatarUrl;
    QString m_nickname;
    QString m_status;
    QString m_feeling;
    int m_tabCurrentIndex = 0;
    QMap<int, QPushButton*> m_tabBtnMap;
    MessageWidget* m_messageWidget = nullptr;
    CotPWidget* m_cotPWidget = nullptr;
    QVBoxLayout* m_exploreLayout = nullptr;
};

#endif
