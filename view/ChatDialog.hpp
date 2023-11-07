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
    void SIG_openLoading();
    void SIG_closeLoading();
    void SIG_clearAllElement();
    void SIG_scrollToBottom();

    void SIG_addTimeline(const QString& time);

    void SIG_addSelfTextMsg(const QString& avatarUrl, const QString& message);
    void SIG_addSelfImageMsg(const QString& avatarUrl, const QString& imageUrl);
    void SIG_addSelfVideoMsg(const QString& avatarUrl, const QString& videoUrl);
    void SIG_addSelfFileMsg(const QString& avatarUrl, const QString& fileUrl);
    void SIG_addPeerTextMsg(const QString& avatarUrl, const QString& message);
    void SIG_addPeerImageMsg(const QString& avatarUrl, const QString& imageUrl);
    void SIG_addPeerVideoMsg(const QString& avatarUrl, const QString& videoUrl);
    void SIG_addPeerFileMsg(const QString& avatarUrl, const QString& fileUrl);

    void SIG_addSelfTextMsgGroup(const QString& avatarUrl, const QString& message);
    void SIG_addSelfImageMsgGroup(const QString& avatarUrl, const QString& imageUrl);
    void SIG_addSelfVideoMsgGroup(const QString& avatarUrl, const QString& videoUrl);
    void SIG_addSelfFileMsgGroup(const QString& avatarUrl, const QString& fileUrl);
    void SIG_addPeerTextMsgGroup(const QString& nickname, const QString& avatarUrl, const QString& message);
    void SIG_addPeerImageMsgGroup(const QString& nickname, const QString& avatarUrl, const QString& imageUrl);
    void SIG_addPeerVideoMsgGroup(const QString& nickname, const QString& avatarUrl, const QString& videoUrl);
    void SIG_addPeerFileMsgGroup(const QString& nickname, const QString& avatarUrl, const QString& fileUrl);

public slots:
    static void download(const QString& url);

    static void previewImg(const QString& url);

private:
    QWebEnginePage* m_page;
};

class WebHelper {
public:
    static WebController* webController;

    static void openLoading();

    static void closeLoading();

    static void clearAllElement();

    static void scrollToBottom();

    static void addSelfPersonMsg(const PersonMessage& pm);

    static void addPeerPersonMsg(const PersonMessage& pm);

    static void addSelfGroupMsg(const GroupMessage& gm);

    static void addPeerGroupMsg(const GroupMessage& gm);

    static void addTimeline(const QString& time);

    static void addSelfTextMsg(const QString& message);

    static void addSelfImageMsg(const QString& imageUrl);

    static void addSelfVideoMsg(const QString& videoUrl);

    static void addSelfFileMsg(const QString& fileUrl);

    static void addPeerTextMsg(int userId, const QString& message);

    static void addPeerImageMsg(int userId, const QString& imageUrl);

    static void addPeerVideoMsg(int userId, const QString& videoUrl);

    static void addPeerFileMsg(int userId, const QString& fileUrl);

    static void addSelfTextMsgGroup(const QString& message);

    static void addSelfImageMsgGroup(const QString& imageUrl);

    static void addSelfVideoMsgGroup(const QString& videoUrl);

    static void addSelfFileMsgGroup(const QString& fileUrl);

    static void addPeerTextMsgGroup(const QString& nickname, const QString& avatar, const QString& message);

    static void addPeerImageMsgGroup(const QString& nickname, const QString& avatar, const QString& imageUrl);

    static void addPeerVideoMsgGroup(const QString& nickname, const QString& avatar, const QString& videoUrl);

    static void addPeerFileMsgGroup(const QString& nickname, const QString& avatar, const QString& fileUrl);
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

    void addChatItem(int id, int mode);

    void changeChatHistory(int id, int mode);

    [[nodiscard]] int getCurrentPeerId() const;

    [[nodiscard]] int getCurrentGroupId() const;

private:
    void handleClickSendPushButton();

private:
    Ui::ChatDialog* ui;
    int m_mode = 0;
    int m_currentPeerId = -1;
    int m_currentGroupId = -1;
    CustomWebEngineView* m_friendChatView = nullptr;
    QWebChannel* m_webChannel = nullptr;
    QMap<int, ChatItem*> m_personChatItemMap;
    QMap<int, ChatItem*> m_groupChatItemMap;
    QVBoxLayout* m_chatItemLayout = nullptr;
    QVector<PersonMessage> m_personMessages;
    QVector<GroupMessage> m_groupMessages;
    WebController* m_webController;
};

#endif
