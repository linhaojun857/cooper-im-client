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

void WebHelper::addSelfMsg(const PersonMessage& pm) {
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

void WebHelper::addPeerMsg(const PersonMessage& pm) {
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

WebController* WebHelper::webController = nullptr;

void WebHelper::addPeerTextMsg(int userId, const QString& text) {
    emit webController->SIG_addPeerTextMsg(IMStore::getInstance()->getFriend(userId)->avatar, text);
}

void WebHelper::addPeerImageMsg(int userId, const QString& url) {
    emit webController->SIG_addPeerImageMsg(IMStore::getInstance()->getFriend(userId)->avatar, url);
}

void WebHelper::addPeerVideoMsg(int userId, const QString& url) {
    emit webController->SIG_addPeerVideoMsg(IMStore::getInstance()->getFriend(userId)->avatar, url);
}

void WebHelper::addPeerFileMsg(int userId, const QString& url) {
    emit webController->SIG_addPeerFileMsg(IMStore::getInstance()->getFriend(userId)->avatar, url);
}

void WebHelper::addSelfTextMsg(const QString& text) {
    emit webController->SIG_addSelfTextMsg(IMStore::getInstance()->getSelf()->avatar, text);
}

void WebHelper::addSelfImageMsg(const QString& url) {
    emit webController->SIG_addSelfImageMsg(IMStore::getInstance()->getSelf()->avatar, url);
}

void WebHelper::addSelfVideoMsg(const QString& url) {
    emit webController->SIG_addSelfVideoMsg(IMStore::getInstance()->getSelf()->avatar, url);
}

void WebHelper::addSelfFileMsg(const QString& url) {
    emit webController->SIG_addSelfFileMsg(IMStore::getInstance()->getSelf()->avatar, url);
}

void WebHelper::clearAllElement() {
    emit webController->SIG_clearAllElement();
}

void WebHelper::openLoading() {
    emit webController->SIG_openLoading();
}

void WebHelper::closeLoading() {
    emit webController->SIG_closeLoading();
}

void WebHelper::scrollToBottom() {
    emit webController->SIG_scrollToBottom();
}

ChatDialog::ChatDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
    setWindowTitle("Cooper");
    setWindowIcon(QIcon(":/img/logo.ico"));
    connect(ui->m_closePushButton, &QPushButton::clicked, [this]() {
        qDebug() << "ui->m_closePushButton, &QPushButton::clicked";
        m_currentPeerId = -1;
        for (const auto& item : m_chatItemMap) {
            IMStore::getInstance()->closeChatPage(item->getId());
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

void ChatDialog::addChatItem(int id) {
    qDebug() << "ChatDialog::addChatItem";
    if (m_chatItemMap.contains(id)) {
        m_chatItemMap[id]->show();
        return;
    }
    auto chatItem = new ChatItem();
    chatItem->setId(id);
    chatItem->setAvatar(IMStore::getInstance()->getFriend(id)->avatar);
    chatItem->setName(IMStore::getInstance()->getFriend(id)->nickname);
    chatItem->setRecentMsg(IMStore::getInstance()->getLatestPersonMessageByUserId(id));
    m_chatItemMap.insert(id, chatItem);
    m_chatItemLayout->insertWidget(m_chatItemLayout->count() - 1, chatItem);
}

void ChatDialog::changeChatHistory(int userId) {
    qDebug() << "ChatDialog::changeChatHistory";
    if (m_currentPeerId == userId) {
        return;
    }
    m_currentPeerId = userId;
    WebHelper::clearAllElement();
    ui->m_nameLabel->setText(m_chatItemMap[userId]->getName());
    QSqlQuery query(*IMStore::getInstance()->getDatabase());
    QString sql = QString(
                      "select * from person_message where (from_id = %1 and to_id = %2) "
                      "or (from_id = %3 and to_id = %4)")
                      .arg(IMStore::getInstance()->getSelf()->id)
                      .arg(userId)
                      .arg(userId)
                      .arg(IMStore::getInstance()->getSelf()->id);
    if (!query.exec(sql)) {
        qDebug() << query.lastError().text();
        return;
    }
    while (query.next()) {
        PersonMessage pm(query.value(1).toInt(), query.value(2).toInt(), query.value(3).toInt(), query.value(4).toInt(),
                         query.value(5).toString(), query.value(6).toString(), query.value(7).toLongLong());
        m_personMessages.push_back(pm);
        if (pm.from_id == IMStore::getInstance()->getSelf()->id) {
            WebHelper::addSelfMsg(pm);
        } else {
            WebHelper::addPeerMsg(pm);
        }
        WebHelper::scrollToBottom();
    }
}

int ChatDialog::getCurrentPeerId() const {
    return m_currentPeerId;
}

void ChatDialog::handleClickSendPushButton() const {
    qDebug() << "ChatDialog::handleClickSendPushButton";
    PersonMessage pm;
    pm.from_id = IMStore::getInstance()->getSelf()->id;
    pm.to_id = m_currentPeerId;
    pm.message_type = MSG_TYPE_TEXT;
    pm.message = ui->m_plainTextEdit->toPlainText();
    pm.timestamp = time(nullptr);
    IMStore::getInstance()->getIMKernel()->sendPersonMsg(pm);
    WebHelper::addSelfMsg(pm);
    WebHelper::scrollToBottom();
}
