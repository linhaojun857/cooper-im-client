#include "MessageWidget.hpp"

#include "store/IMStore.hpp"
#include "ui_MessageWidget.h"

MessageWidget::MessageWidget(QWidget* parent) : QWidget(parent), ui(new Ui::MessageWidget) {
    ui->setupUi(this);
    IMStore::getInstance()->setMessageWidget(this);
    ui->m_scrollArea->setFrameStyle(QFrame::NoFrame);
    ui->m_scrollArea->setFrameShape(QFrame::NoFrame);
    ui->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto widget = new QWidget();
    ui->m_scrollArea->setWidget(widget);
    m_messageItemLayout = new QVBoxLayout();
    m_messageItemLayout->setContentsMargins(0, 0, 0, 0);
    m_messageItemLayout->setSpacing(0);
    widget->setLayout(m_messageItemLayout);
    m_messageItemLayout->addStretch();
}

MessageWidget::~MessageWidget() {
    delete ui;
}

void MessageWidget::addMessageItem(MessageItem* messageItem) {
    qDebug() << "MessageWidget::addMessageItem";
    m_messageItemLayout->insertWidget(m_messageItemLayout->count() - 1, messageItem);
}

void MessageWidget::addMessageItem1(MessageItem* messageItem) {
    qDebug() << "MessageWidget::addMessageItem1";
    for (int i = 0; i < m_messageItemLayout->count(); i++) {
        auto item = dynamic_cast<MessageItem*>(m_messageItemLayout->itemAt(i)->widget());
        if (item && item->getTime() < messageItem->getTime()) {
            m_messageItemLayout->insertWidget(i, messageItem);
            return;
        }
    }
}

void MessageWidget::moveMessageItemToTop(MessageItem* messageItem) {
    qDebug() << "MessageWidget::moveMessageItemToTop";
    m_messageItemLayout->removeWidget(messageItem);
    m_messageItemLayout->insertWidget(0, messageItem);
}
