#ifndef view_MessageWidget_hpp
#define view_MessageWidget_hpp

#include <QVBoxLayout>
#include <QWidget>

#include "view/MessageItem.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MessageWidget;
}
QT_END_NAMESPACE

class MessageWidget : public QWidget {
    Q_OBJECT

public:
    explicit MessageWidget(QWidget* parent = nullptr);

    ~MessageWidget() override;

    void addMessageItem(MessageItem* messageItem);

    void moveMessageItemToTop(MessageItem* messageItem);

private:
    Ui::MessageWidget* ui;
    QVBoxLayout* m_messageItemLayout = nullptr;
};

#endif
