#ifndef view_ChatDialog_hpp
#define view_ChatDialog_hpp

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QWebEngineView>

#include "view/ChatItem.hpp"

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

    void addChatItem(ChatItem* chatItem);

    void changeChatHistory(int userId);

private:
    void runJavaScript(const QString& script);

private:
    Ui::ChatDialog* ui;
    QWebEngineView* m_friendChatView = nullptr;
    QMap<int, ChatItem*> m_chatItemMap;
    QVBoxLayout* m_chatItemLayout = nullptr;
};

#endif
