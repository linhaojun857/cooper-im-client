#ifndef view_CotPWidget_HPP_
#define view_CotPWidget_HPP_

#include <QWidget>
#include <QPushButton>

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
};

#endif
