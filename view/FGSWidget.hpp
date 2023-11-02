#ifndef view_FGSWidget_hpp
#define view_FGSWidget_hpp

#include <QGridLayout>
#include <QPushButton>
#include <QWidget>

#include "view/FSRItem.hpp"
#include "view/GSRItem.hpp"

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

    void clearAllGSRItems();

    void addFSRItem(FSRItem* fsrItem, int row, int column);

    void addGSRItem(GSRItem* gsrItem, int row, int column);

private:
    void handleUserClickSearchPushButton();

private:
    Ui::FGSWidget* ui;
    int m_tabCurrentIndex = 0;
    QMap<int, QPushButton*> m_tabBtnMap;
    int m_searchMode = 0;
    QList<FSRItem*> m_fsrItems;
    QList<GSRItem*> m_gsrItems;
    QGridLayout* m_fsrItemsLayout = nullptr;
    QGridLayout* m_gsrItemsLayout = nullptr;
};

#endif
