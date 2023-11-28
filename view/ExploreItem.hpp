#ifndef view_ExploreItem_hpp
#define view_ExploreItem_hpp

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class ExploreItem;
}
QT_END_NAMESPACE

class ExploreItem : public QWidget {
    Q_OBJECT

public:
    explicit ExploreItem(QWidget* parent = nullptr);

    ~ExploreItem() override;

    void setName(const QString& name);

signals:
    void clicked();

private:
    Ui::ExploreItem* ui;
};

#endif
