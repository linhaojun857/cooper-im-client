#ifndef view_FGSWidget_hpp
#define view_FGSWidget_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class FGSWidget;
}
QT_END_NAMESPACE

class FGSWidget : public QWidget {
    Q_OBJECT

public:
    explicit FGSWidget(QWidget* parent = nullptr);

    ~FGSWidget() override;

private:
    Ui::FGSWidget* ui;
};

#endif
