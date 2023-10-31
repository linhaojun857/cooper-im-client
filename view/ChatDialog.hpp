#ifndef view_ChatDialog_hpp
#define view_ChatDialog_hpp

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineView>
#include <QWheelEvent>

#include "entity/Entity.hpp"
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

signals:
    void SIG_addPeerTextMsg(const QString& avatar, const QString& text);
    void SIG_addPeerImageMsg(const QString& avatar, const QString& url);
    void SIG_addPeerVideoMsg(const QString& avatar, const QString& url);
    void SIG_addPeerFileMsg(const QString& avatar, const QString& url);
    void SIG_addSelfTextMsg(const QString& avatar, const QString& text);
    void SIG_addSelfImageMsg(const QString& avatar, const QString& url);
    void SIG_addSelfVideoMsg(const QString& avatar, const QString& url);
    void SIG_addSelfFileMsg(const QString& avatar, const QString& url);
    void SIG_clearAllElement();
    void SIG_openLoading();
    void SIG_closeLoading();
    void SIG_scrollToBottom();

public slots:
    static void download(const QString& url);

    static void previewImg(const QString& url);

private:
    QWebEnginePage* m_page;
};

class WebHelper {
public:
    static WebController* webController;

    static void addSelfMsg(const PersonMessage& pm);

    static void addPeerMsg(const PersonMessage& pm);

    static void addPeerTextMsg(int userId, const QString& text);

    static void addPeerImageMsg(int userId, const QString& url);

    static void addPeerVideoMsg(int userId, const QString& url);

    static void addPeerFileMsg(int userId, const QString& url);

    static void addSelfTextMsg(const QString& text);

    static void addSelfImageMsg(const QString& url);

    static void addSelfVideoMsg(const QString& url);

    static void addSelfFileMsg(const QString& url);

    static void clearAllElement();

    static void openLoading();

    static void closeLoading();

    static void scrollToBottom();
};

class CustomWebEngineView : public QWebEngineView {
public:
    explicit CustomWebEngineView(QWidget* parent = nullptr) : QWebEngineView(parent) {
    }
    bool event(QEvent* event) override {
        if (event->type() == QEvent::ChildPolished) {
            auto* ce = dynamic_cast<QChildEvent*>(event);
            child = ce->child();
            if (child) {
                child->installEventFilter(this);
            }
        }
        return QWebEngineView::event(event);
    }

    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::Wheel) {
            if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
                return true;
            }
        }
        return QWebEngineView::eventFilter(obj, event);
    }

private:
    QObject* child = nullptr;
};

class ChatDialog : public QDialog {
    Q_OBJECT

    friend class WebHelper;

public:
    explicit ChatDialog(QWidget* parent = nullptr);

    ~ChatDialog() override;

    void hideDialog();

    void showDialog();

    void addChatItem(int id);

    void changeChatHistory(int userId);

    [[nodiscard]] int getCurrentPeerId() const;

private:
    void handleClickSendPushButton() const;

private:
    Ui::ChatDialog* ui;
    int m_currentPeerId = 0;
    CustomWebEngineView* m_friendChatView = nullptr;
    QWebChannel* m_webChannel = nullptr;
    QMap<int, ChatItem*> m_chatItemMap;
    QVBoxLayout* m_chatItemLayout = nullptr;
    QVector<PersonMessage> m_personMessages;
    WebController* m_webController;
};

#endif
