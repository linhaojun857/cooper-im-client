#ifndef view_ChatDialog_hpp
#define view_ChatDialog_hpp

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineView>

#include "view/ChatItem.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class ChatDialog;
}
QT_END_NAMESPACE

class WebController : public QObject {
    Q_OBJECT
public:
    void setPage(QWebEnginePage* page);

public slots:
    static void download(const QString& url);

    static void previewImg(const QString& url);

private:
    QWebEnginePage* m_page;
};

class MsgHelper {
public:
    static void addPeerTextMsg(int userId, const QString& text);

    static void addPeerImageMsg(int userId, const QString& url);

    static void addPeerVideoMsg(int userId, const QString& url);

    static void addPeerFileMsg(int userId, const QString& url);

    static void addSelfTextMsg(const QString& text);

    static void addSelfImageMsg(const QString& url);

    static void addSelfVideoMsg(const QString& url);

    static void addSelfFileMsg(const QString& url);
};

class ChatDialog : public QDialog {
    Q_OBJECT

    friend class MsgHelper;

public:
    explicit ChatDialog(QWidget* parent = nullptr);

    ~ChatDialog() override;

    void addChatItem(ChatItem* chatItem);

    void changeChatHistory(int userId);

    [[nodiscard]] int getCurrentPeerId() const;

private:
    void runJavaScript(const QString& script);

    void handleClickSendPushButton() const;

private:
    Ui::ChatDialog* ui;
    int m_currentPeerId = 0;
    QWebEngineView* m_friendChatView = nullptr;
    QWebChannel* m_webChannel = nullptr;
    QMap<int, ChatItem*> m_chatItemMap;
    QVBoxLayout* m_chatItemLayout = nullptr;
};

#endif
