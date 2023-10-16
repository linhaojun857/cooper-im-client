#ifndef view_CotPWidget_HPP
#define view_CotPWidget_HPP

#include <QPushButton>
#include <QWidget>

#include "view/FriendWidget.hpp"
#include "view/GroupWidget.hpp"
#include "view/NotifyWidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class CotPWidget;
}
QT_END_NAMESPACE

class CotPWidget : public QWidget {
    Q_OBJECT

public:
    explicit CotPWidget(QWidget* parent = nullptr);

    ~CotPWidget() override;

private:
    Ui::CotPWidget* ui;
    int m_tabCurrentIndex = 0;
    QMap<int, QPushButton*> m_tabBtnMap;
    FriendWidget* m_friendWidget = nullptr;
    GroupWidget* m_groupWidget = nullptr;
    NotifyWidget* m_notifyWidget = nullptr;
};

#endif
