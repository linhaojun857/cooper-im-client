#ifndef view_ChatDialog_hpp
#define view_ChatDialog_hpp

#include <QDialog>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class ChatDialog;
}
QT_END_NAMESPACE

class ChatDialog : public QDialog {
    Q_OBJECT

public:
    explicit ChatDialog(QWidget* parent = nullptr);

    ~ChatDialog() override;

private:
    Ui::ChatDialog* ui;
    QWidget* m_headerWidget = nullptr;
};

#endif
