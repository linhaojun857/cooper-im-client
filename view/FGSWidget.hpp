#ifndef view_FGSWidget_hpp
#define view_FGSWidget_hpp

#include <QGridLayout>
#include <QPushButton>
#include <QWidget>

#include "view/FSRItem.hpp"

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

    void clearAllFSRItems();

    void addFSRItem(FSRItem* fsrItem, int row, int column);

private:
    void handleUserClickSearchPushButton();

private:
    Ui::FGSWidget* ui;
    int m_tabCurrentIndex = 0;
    QMap<int, QPushButton*> m_tabBtnMap;
    int m_searchMode = 0;
    QList<FSRItem*> m_fsrItems;
    QGridLayout* m_fsrItemsLayout = nullptr;
};

#endif
