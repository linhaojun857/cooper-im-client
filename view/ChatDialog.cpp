#include "ChatDialog.hpp"

#include <FramelessHelper/Widgets/framelesswidgetshelper.h>

#include <QFile>
#include <QFileDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkProxyFactory>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>

#include "core/IMKernel.hpp"
#include "mock/Mock.hpp"
#include "store/IMStore.hpp"
#include "ui_ChatDialog.h"
#include "view/ImageViewWidget.hpp"

FRAMELESSHELPER_USE_NAMESPACE

void WebController::setPage(QWebEnginePage* page) {
    m_page = page;
}

void WebController::download(const QString& url) {
    auto tempFileName = url.split("/").last();
    auto fileName = QFileDialog::getSaveFileName(nullptr, "save file", tempFileName, "All Files(*)");
    std::ignore = QtConcurrent::run([=]() {
        auto manager = new QNetworkAccessManager();
        QNetworkRequest request(url);
        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        file.write(reply->readAll());
        file.close();
    });
}

void WebController::previewImg(const QString& url) {
    static auto imageViewWidget = new ImageViewWidget();
    imageViewWidget->clearImage();
    imageViewWidget->setImage(url);
    imageViewWidget->show();
}

WebController* WebHelper::webController = nullptr;

void WebHelper::addSelfPersonMsg(const PersonMessage& pm) {
    switch (pm.message_type) {
        case MSG_TYPE_TEXT:
            addSelfTextMsg(pm.message);
            break;
        case MSG_TYPE_IMAGE:
            addSelfImageMsg(pm.file_url);
            break;
        case MSG_TYPE_ViDEO:
            addSelfVideoMsg(pm.file_url);
            break;
        case MSG_TYPE_FILE:
            addSelfFileMsg(pm.file_url);
            break;
    }
}

void WebHelper::addPeerPersonMsg(const PersonMessage& pm) {
    switch (pm.message_type) {
        case MSG_TYPE_TEXT:
            addPeerTextMsg(pm.from_id, pm.message);
            break;
        case MSG_TYPE_IMAGE:
            addPeerImageMsg(pm.from_id, pm.file_url);
            break;
        case MSG_TYPE_ViDEO:
            addPeerVideoMsg(pm.from_id, pm.file_url);
            break;
        case MSG_TYPE_FILE:
            addPeerFileMsg(pm.from_id, pm.file_url);
            break;
    }
}

void WebHelper::addSelfGroupMsg(const GroupMessage& gm) {
    switch (gm.message_type) {
        case MSG_TYPE_TEXT:
            addSelfTextMsgGroup(gm.message);
            break;
        case MSG_TYPE_IMAGE:
            addSelfImageMsgGroup(gm.file_url);
            break;
        case MSG_TYPE_ViDEO:
            addSelfVideoMsgGroup(gm.file_url);
            break;
        case MSG_TYPE_FILE:
            addSelfFileMsgGroup(gm.file_url);
            break;
    }
}

void WebHelper::addPeerGroupMsg(const GroupMessage& gm) {
    switch (gm.message_type) {
        case MSG_TYPE_TEXT:
            addPeerTextMsgGroup(gm.from_nickname, gm.from_avatar, gm.message);
            break;
        case MSG_TYPE_IMAGE:
            addPeerImageMsgGroup(gm.from_nickname, gm.from_avatar, gm.file_url);
            break;
        case MSG_TYPE_ViDEO:
            addPeerVideoMsgGroup(gm.from_nickname, gm.from_avatar, gm.file_url);
            break;
        case MSG_TYPE_FILE:
            addPeerFileMsgGroup(gm.from_nickname, gm.from_avatar, gm.file_url);
            break;
    }
}

void WebHelper::addTimeline(const QString& time) {
    emit webController->SIG_addTimeline(time);
}

void WebHelper::openLoading() {
    emit webController->SIG_openLoading();
}

void WebHelper::closeLoading() {
    emit webController->SIG_closeLoading();
}

void WebHelper::clearAllElement() {
    emit webController->SIG_clearAllElement();
}

void WebHelper::scrollToBottom() {
    emit webController->SIG_scrollToBottom();
}

void WebHelper::addSelfTextMsg(const QString& message) {
    emit webController->SIG_addSelfTextMsg(IMStore::getInstance()->getSelf()->avatar, message);
}

void WebHelper::addSelfImageMsg(const QString& imageUrl) {
    emit webController->SIG_addSelfImageMsg(IMStore::getInstance()->getSelf()->avatar, imageUrl);
}

void WebHelper::addSelfVideoMsg(const QString& videoUrl) {
    emit webController->SIG_addSelfVideoMsg(IMStore::getInstance()->getSelf()->avatar, videoUrl);
}

void WebHelper::addSelfFileMsg(const QString& fileUrl) {
    emit webController->SIG_addSelfFileMsg(IMStore::getInstance()->getSelf()->avatar, fileUrl);
}

void WebHelper::addPeerTextMsg(int userId, const QString& message) {
    emit webController->SIG_addPeerTextMsg(IMStore::getInstance()->getFriend(userId)->avatar, message);
}

void WebHelper::addPeerImageMsg(int userId, const QString& imageUrl) {
    emit webController->SIG_addPeerImageMsg(IMStore::getInstance()->getFriend(userId)->avatar, imageUrl);
}

void WebHelper::addPeerVideoMsg(int userId, const QString& videoUrl) {
    emit webController->SIG_addPeerVideoMsg(IMStore::getInstance()->getFriend(userId)->avatar, videoUrl);
}

void WebHelper::addPeerFileMsg(int userId, const QString& fileUrl) {
    emit webController->SIG_addPeerFileMsg(IMStore::getInstance()->getFriend(userId)->avatar, fileUrl);
}

void WebHelper::addSelfTextMsgGroup(const QString& message) {
    emit webController->SIG_addSelfTextMsgGroup(IMStore::getInstance()->getSelf()->avatar, message);
}

void WebHelper::addSelfImageMsgGroup(const QString& imageUrl) {
    emit webController->SIG_addSelfImageMsgGroup(IMStore::getInstance()->getSelf()->avatar, imageUrl);
}

void WebHelper::addSelfVideoMsgGroup(const QString& videoUrl) {
    emit webController->SIG_addSelfVideoMsgGroup(IMStore::getInstance()->getSelf()->avatar, videoUrl);
}

void WebHelper::addSelfFileMsgGroup(const QString& fileUrl) {
    emit webController->SIG_addSelfFileMsgGroup(IMStore::getInstance()->getSelf()->avatar, fileUrl);
}

void WebHelper::addPeerTextMsgGroup(const QString& nickname, const QString& avatar, const QString& message) {
    emit webController->SIG_addPeerTextMsgGroup(nickname, avatar, message);
}

void WebHelper::addPeerImageMsgGroup(const QString& nickname, const QString& avatar, const QString& imageUrl) {
    emit webController->SIG_addPeerImageMsgGroup(nickname, avatar, imageUrl);
}

void WebHelper::addPeerVideoMsgGroup(const QString& nickname, const QString& avatar, const QString& videoUrl) {
    emit webController->SIG_addPeerVideoMsgGroup(nickname, avatar, videoUrl);
}

void WebHelper::addPeerFileMsgGroup(const QString& nickname, const QString& avatar, const QString& fileUrl) {
    emit webController->SIG_addPeerFileMsgGroup(nickname, avatar, fileUrl);
}

ChatDialog::ChatDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
    setWindowTitle("Cooper");
    setWindowIcon(QIcon(":/img/logo.ico"));
    connect(ui->m_closePushButton, &QPushButton::clicked, [this]() {
        qDebug() << "ui->m_closePushButton, &QPushButton::clicked";
        m_currentPeerId = -1;
        m_currentGroupId = -1;
        for (const auto& item : m_personChatItemMap) {
            IMStore::getInstance()->closePersonChatPage(item->getId());
            item->hide();
        }
        for (const auto& item : m_groupChatItemMap) {
            IMStore::getInstance()->closeGroupChatPage(item->getId());
            item->hide();
        }
        hideDialog();
    });
    connect(ui->m_minimizePushButton, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));
    connect(ui->m_sendPushButton, &QPushButton::clicked, this, &ChatDialog::handleClickSendPushButton);

    FramelessWidgetsHelper::get(this)->extendsContentIntoTitleBar();
    FramelessWidgetsHelper::get(this)->setTitleBarWidget(ui->m_headerWidget);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(ui->m_closePushButton);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(ui->m_minimizePushButton);

    QFile file(":/html/chat.html");
    file.open(QIODevice::ReadOnly);
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    m_friendChatView = new CustomWebEngineView(this);
    m_friendChatView->setHtml(file.readAll());
    m_friendChatView->setGeometry(200, 30, 560, 317);
    m_webChannel = new QWebChannel(m_friendChatView->page());
    m_webController = new WebController();
    WebHelper::webController = m_webController;
    m_webController->setPage(m_friendChatView->page());
    m_webChannel->registerObject(QStringLiteral("webController"), m_webController);
    m_friendChatView->page()->setWebChannel(m_webChannel);

    ui->m_scrollArea->setFrameStyle(QFrame::NoFrame);
    ui->m_scrollArea->setFrameShape(QFrame::NoFrame);
    ui->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto widget = new QWidget();
    ui->m_scrollArea->setWidget(widget);
    m_chatItemLayout = new QVBoxLayout();
    m_chatItemLayout->setContentsMargins(0, 0, 0, 0);
    m_chatItemLayout->setSpacing(0);
    widget->setLayout(m_chatItemLayout);
    m_chatItemLayout->addStretch();
}

ChatDialog::~ChatDialog() {
    delete ui;
    delete m_friendChatView;
}

void ChatDialog::hideDialog() {
    WebHelper::clearAllElement();
    m_friendChatView->hide();
    this->repaint();
    QTimer::singleShot(20, [this]() {
        hide();
    });
}

void ChatDialog::showDialog() {
    m_friendChatView->show();
    this->show();
}

void ChatDialog::addChatItem(int id, int mode) {
    qDebug() << "ChatDialog::addChatItem";
    if (mode == 0) {
        if (m_personChatItemMap.contains(id)) {
            m_personChatItemMap[id]->show();
            return;
        }
        auto chatItem = new ChatItem();
        chatItem->setMode(0);
        chatItem->setId(id);
        chatItem->setAvatar(IMStore::getInstance()->getFriend(id)->avatar);
        chatItem->setName(IMStore::getInstance()->getFriend(id)->nickname);
        chatItem->setRecentMsg(IMStore::getInstance()->getLatestPersonMessageByUserId(id));
        m_personChatItemMap.insert(id, chatItem);
        m_chatItemLayout->insertWidget(m_chatItemLayout->count() - 1, chatItem);
    } else {
        if (m_groupChatItemMap.contains(id)) {
            m_groupChatItemMap[id]->show();
            return;
        }
        auto chatItem = new ChatItem();
        chatItem->setMode(1);
        chatItem->setId(id);
        chatItem->setAvatar(IMStore::getInstance()->getGroup(id)->avatar);
        chatItem->setName(IMStore::getInstance()->getGroup(id)->name);
        chatItem->setRecentMsg(IMStore::getInstance()->getLatestGroupMessageByGroupId(id));
        m_groupChatItemMap.insert(id, chatItem);
        m_chatItemLayout->insertWidget(m_chatItemLayout->count() - 1, chatItem);
    }
}

void ChatDialog::changeChatHistory(int id, int mode) {
    qDebug() << "ChatDialog::changeChatHistory";
    if (mode == 0) {
        if (m_currentPeerId == id) {
            return;
        }
        m_currentPeerId = id;
        m_currentGroupId = -1;
        m_mode = 0;
        WebHelper::clearAllElement();
        ui->m_nameLabel->setText(m_personChatItemMap[id]->getName());
        QSqlQuery query(*IMStore::getInstance()->getDatabase());
        QString sql1 = QString("select session_id from t_friend where friend_id = %1").arg(id);
        if (!query.exec(sql1)) {
            qDebug() << query.lastError().text();
            return;
        }
        if (!query.next()) {
            return;
        }
        QString sessionId = query.value(0).toString();
        QString sql2 = QString("select * from t_person_message where session_id = '%1'").arg(sessionId);
        if (!query.exec(sql2)) {
            qDebug() << query.lastError().text();
            return;
        }
        while (query.next()) {
            PersonMessage pm(query.value(1).toInt(), query.value(2).toInt(), query.value(3).toInt(),
                             query.value(4).toString(), query.value(5).toInt(), query.value(6).toString(),
                             query.value(7).toString(), query.value(8).toLongLong());
            m_personMessages.push_back(pm);
            if (pm.from_id == IMStore::getInstance()->getSelf()->id) {
                WebHelper::addSelfPersonMsg(pm);
            } else {
                WebHelper::addPeerPersonMsg(pm);
            }
            WebHelper::scrollToBottom();
        }
    } else {
        if (m_currentGroupId == id) {
            return;
        }
        m_currentGroupId = id;
        m_currentPeerId = -1;
        m_mode = 1;
        WebHelper::clearAllElement();
        ui->m_nameLabel->setText(m_groupChatItemMap[id]->getName());
        QSqlQuery query(*IMStore::getInstance()->getDatabase());
        QString sql = QString("select * from t_group_message where group_id = %1").arg(id);
        if (!query.exec(sql)) {
            qDebug() << query.lastError().text();
            return;
        }
        while (query.next()) {
            GroupMessage gm(query.value(1).toInt(), query.value(2).toInt(), query.value(3).toString(),
                            query.value(4).toString(), query.value(5).toInt(), query.value(6).toInt(),
                            query.value(7).toString(), query.value(8).toString(), query.value(9).toLongLong());
            if (gm.from_id == IMStore::getInstance()->getSelf()->id) {
                WebHelper::addSelfGroupMsg(gm);
            } else {
                WebHelper::addPeerGroupMsg(gm);
            }
            WebHelper::scrollToBottom();
        }
    }
}

int ChatDialog::getCurrentPeerId() const {
    return m_currentPeerId;
}

int ChatDialog::getCurrentGroupId() const {
    return m_currentGroupId;
}

void ChatDialog::handleClickSendPushButton() {
    qDebug() << "ChatDialog::handleClickSendPushButton";
    QString message = ui->m_plainTextEdit->toPlainText();
    QString messageTemp = message;
    if (message.isEmpty() || messageTemp.replace(" ", "").replace("\n", "").isEmpty()) {
        QMessageBox::warning(this, "提示", "不能发送空消息");
        return;
    }
    if (m_mode == 0) {
        PersonMessage pm;
        pm.from_id = IMStore::getInstance()->getSelf()->id;
        pm.to_id = m_currentPeerId;
        pm.message_type = MSG_TYPE_TEXT;
        pm.message = message;
        pm.timestamp = time(nullptr);
        IMStore::getInstance()->getIMKernel()->sendPersonMsg(pm);
        WebHelper::addSelfPersonMsg(pm);
    } else {
        GroupMessage gm;
        gm.from_id = IMStore::getInstance()->getSelf()->id;
        gm.group_id = m_currentGroupId;
        gm.message_type = MSG_TYPE_TEXT;
        gm.message = message;
        gm.timestamp = time(nullptr);
        IMStore::getInstance()->getIMKernel()->sendGroupMsg(gm);
        WebHelper::addSelfGroupMsg(gm);
    }
    WebHelper::scrollToBottom();
}
