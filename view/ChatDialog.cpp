#include "ChatDialog.hpp"

#include <FramelessHelper/Widgets/framelesswidgetshelper.h>

#include <QFile>
#include <QFileDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QNetworkAccessManager>
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

void MsgHelper::addPeerTextMsg(int userId, const QString& text) {
    auto fri = IMStore::getInstance()->getFriend(userId);
    auto chatDialog = IMStore::getInstance()->getChatDialog();
    chatDialog->runJavaScript(
        QString("addPeerImageMessage(%1, %2);").arg("\"" + fri->avatar + "\"", "\"" + text + "\""));
    chatDialog->runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
}

void MsgHelper::addPeerImageMsg(int userId, const QString& url) {
    auto fri = IMStore::getInstance()->getFriend(userId);
    auto chatDialog = IMStore::getInstance()->getChatDialog();
    chatDialog->runJavaScript(
        QString("addPeerImageMessage(%1, %2);").arg("\"" + fri->avatar + "\"", "\"" + url + "\""));
    chatDialog->runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
}

void MsgHelper::addPeerVideoMsg(int userId, const QString& url) {
    auto fri = IMStore::getInstance()->getFriend(userId);
    auto chatDialog = IMStore::getInstance()->getChatDialog();
    chatDialog->runJavaScript(
        QString("addPeerVideoMessage(%1, %2);").arg("\"" + fri->avatar + "\"", "\"" + url + "\""));
    chatDialog->runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
}

void MsgHelper::addPeerFileMsg(int userId, const QString& url) {
    auto fri = IMStore::getInstance()->getFriend(userId);
    auto chatDialog = IMStore::getInstance()->getChatDialog();
    chatDialog->runJavaScript(QString("addPeerFileMessage(%1, %2);").arg("\"" + fri->avatar + "\"", "\"" + url + "\""));
    chatDialog->runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
}

void MsgHelper::addSelfTextMsg(const QString& text) {
    auto self = IMStore::getInstance()->getSelf();
    auto chatDialog = IMStore::getInstance()->getChatDialog();
    chatDialog->runJavaScript(
        QString("addSelfTextMessage(%1, %2);").arg("\"" + self->avatar + "\"", "\"" + text + "\""));
    chatDialog->runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
}

void MsgHelper::addSelfImageMsg(const QString& url) {
    auto self = IMStore::getInstance()->getSelf();
    auto chatDialog = IMStore::getInstance()->getChatDialog();
    chatDialog->runJavaScript(
        QString("addSelfImageMessage(%1, %2);").arg("\"" + self->avatar + "\"", "\"" + url + "\""));
    chatDialog->runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
}

void MsgHelper::addSelfVideoMsg(const QString& url) {
    auto self = IMStore::getInstance()->getSelf();
    auto chatDialog = IMStore::getInstance()->getChatDialog();
    chatDialog->runJavaScript(
        QString("addSelfVideoMessage(%1, %2);").arg("\"" + self->avatar + "\"", "\"" + url + "\""));
    chatDialog->runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
}

void MsgHelper::addSelfFileMsg(const QString& url) {
    auto self = IMStore::getInstance()->getSelf();
    auto chatDialog = IMStore::getInstance()->getChatDialog();
    chatDialog->runJavaScript(
        QString("addSelfFileMessage(%1, %2);").arg("\"" + self->avatar + "\"", "\"" + url + "\""));
    chatDialog->runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
}

ChatDialog::ChatDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
    setWindowTitle("Cooper");
    setWindowIcon(QIcon(":/img/logo.ico"));
    connect(ui->m_closePushButton, &QPushButton::clicked, [this]() {
        runJavaScript("clearAllElement();");
        m_friendChatView->repaint();
        hide();
        qDebug() << "ui->m_closePushButton, &QPushButton::clicked";
        for (const auto& item : m_chatItemMap) {
            IMStore::getInstance()->closeChatPage(item->getId());
            delete item;
        }
        m_chatItemMap.clear();
    });
    connect(ui->m_minimizePushButton, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));

    FramelessWidgetsHelper::get(this)->extendsContentIntoTitleBar();
    FramelessWidgetsHelper::get(this)->setTitleBarWidget(ui->m_headerWidget);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(ui->m_closePushButton);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(ui->m_minimizePushButton);

    QFile file(":/html/chat.html");
    file.open(QIODevice::ReadOnly);
    m_friendChatView = new QWebEngineView(this);
    m_friendChatView->setHtml(file.readAll());
    m_friendChatView->setGeometry(170, 30, 560, 317);
    m_webChannel = new QWebChannel(m_friendChatView->page());
    auto controller = new WebController();
    controller->setPage(m_friendChatView->page());
    m_webChannel->registerObject(QStringLiteral("webController"), controller);
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

    connect(ui->m_sendPushButton, &QPushButton::clicked, [this]() {
        QString selfAvatarUrl = "\"https://static.linhaojun.top/aurora/config/6e4f47f4bb66d2d30722d20e789b220e.jpg\"";
        runJavaScript(QString("addSelfTextMessage(%1, %2);").arg(selfAvatarUrl, "\"this is test message\""));
        runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
    });
}

ChatDialog::~ChatDialog() {
    delete ui;
}

void ChatDialog::addChatItem(ChatItem* chatItem) {
    qDebug() << "ChatDialog::addChatItem";
    m_chatItemMap.insert(chatItem->getId(), chatItem);
    changeChatHistory(chatItem->getId());
    m_chatItemLayout->insertWidget(m_chatItemLayout->count() - 1, chatItem);
}

void ChatDialog::changeChatHistory(int userId) {
    qDebug() << "ChatDialog::changeChatHistory";
    runJavaScript("clearAllElement();");
    runJavaScript("openLoading();");
    ui->m_nameLabel->setText(m_chatItemMap[userId]->getName());
    int mockId = userId % (int)Mock::chatHistory.size();
    auto chatHistory = Mock::chatHistory[mockId];
    for (const auto& history : chatHistory) {
        runJavaScript(
            QString("addPeerTextMessage(%1, %2);").arg("\"" + m_chatItemMap[userId]->getAvatar() + "\"", history));
    }
    runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
    runJavaScript("closeLoading();");
}

void ChatDialog::runJavaScript(const QString& script) {
    m_friendChatView->page()->runJavaScript(script);
}
