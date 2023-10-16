#include "ChatDialog.hpp"

#include <FramelessHelper/Widgets/framelesswidgetshelper.h>

#include <QFile>
#include <QListWidget>
#include <QMessageBox>

#include "core/IMKernel.hpp"
#include "mock/Mock.hpp"
#include "store/IMStore.hpp"
#include "ui_ChatDialog.h"

FRAMELESSHELPER_USE_NAMESPACE

ChatDialog::ChatDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
    m_headerWidget = new QWidget(this);
    m_headerWidget->setGeometry(170, 0, 480, 30);
    ui->m_peerNicknameLabel->setParent(m_headerWidget);
    ui->m_peerNicknameLabel->setGeometry(205, 5, 150, 20);
    connect(ui->m_closePushButton, &QPushButton::clicked, [this]() {
        runJavaScript("clearAllElement();");
        hide();
        qDebug() << "ui->m_closePushButton, &QPushButton::clicked";
        for (const auto& item : m_chatItemMap.keys()) {
            IMStore::getInstance()->closeChatPage(item);
            delete m_chatItemMap[item];
        }
        m_chatItemMap.clear();
    });
    connect(ui->m_minimizePushButton, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));

    FramelessWidgetsHelper::get(this)->extendsContentIntoTitleBar();
    FramelessWidgetsHelper::get(this)->setTitleBarWidget(m_headerWidget);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(ui->m_closePushButton);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(ui->m_minimizePushButton);

    QFile file(":/html/friend_chat.html");
    file.open(QIODevice::ReadOnly);
    m_friendChatView = new QWebEngineView(this);
    m_friendChatView->setHtml(file.readAll());
    m_friendChatView->setGeometry(170, 30, 560, 317);

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
    int mockId = userId % Mock::chatHistory.size();
    auto chatHistory = Mock::chatHistory[mockId];
    for (const auto& history : chatHistory) {
        qDebug() << m_chatItemMap[userId]->getAvatar();
        qDebug() << history;
        runJavaScript(
            QString("addPeerTextMessage(%1, %2);").arg("\"" + m_chatItemMap[userId]->getAvatar() + "\"", history));
    }
    runJavaScript("window.scrollTo(0, document.body.scrollHeight);");
}

void ChatDialog::runJavaScript(const QString& script) {
    m_friendChatView->page()->runJavaScript(script);
}
