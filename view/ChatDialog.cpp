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

void MsgHelper::addSelfMsg(PersonMessage pm) {
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
            addSelfTextMsg(pm.file_url);
            break;
    }
}

void MsgHelper::addPeerMsg(PersonMessage pm) {
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

void MsgHelper::addPeerTextMsg(int userId, const QString& text) {
    auto fri = IMStore::getInstance()->getFriend(userId);
    auto chatDialog = IMStore::getInstance()->getChatDialog();
    chatDialog->runJavaScript(
        QString("addPeerTextMessage(%1, %2);").arg("\"" + fri->avatar + "\"", "\"" + text + "\""));
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
    connect(ui->m_sendPushButton, &QPushButton::clicked, this, &ChatDialog::handleClickSendPushButton);

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
    m_currentPeerId = userId;
    runJavaScript("clearAllElement();");
    runJavaScript("openLoading();");
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
            MsgHelper::addSelfMsg(pm);
        } else {
            MsgHelper::addPeerMsg(pm);
        }
    }
    runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
    runJavaScript("closeLoading();");
}

int ChatDialog::getCurrentPeerId() const {
    return m_currentPeerId;
}

void ChatDialog::runJavaScript(const QString& script) {
    m_friendChatView->page()->runJavaScript(script);
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
    MsgHelper::addSelfTextMsg(pm.message);
}
