#include "PyqCommentItem.hpp"

#include "ui_PyqCommentItem.h"

PyqCommentItem::PyqCommentItem(QWidget* parent) : QWidget(parent), ui(new Ui::PyqCommentItem) {
    ui->setupUi(this);
}

PyqCommentItem::~PyqCommentItem() {
    delete ui;
}

void PyqCommentItem::setCommonComment(const QString& nickname, const QString& comment) {
    QString content;
    content = content.append(QString("<font color=\"#576b95\">%1</font>").arg(nickname))
                  .append(QString("<font color=\"#000000\">: </font>"))
                  .append(QString("<font color=\"#000000\">%1</font>").arg(comment));
    int oldHeight = ui->m_textBrowser->size().rheight();
    ui->m_textBrowser->setText(content);
    ui->m_textBrowser->document()->setTextWidth(ui->m_textBrowser->width());
    ui->m_textBrowser->setFixedHeight((int)(ui->m_textBrowser->document()->size().height()));
    ui->m_textBrowser->setFixedWidth((int)(ui->m_textBrowser->document()->size().width()));
    int newHeight = ui->m_textBrowser->size().rheight();
    int deltaHeight = newHeight - oldHeight;
    setFixedHeight(this->height() + deltaHeight);
    setMinimumSize(this->width(), this->height());
    setMaximumSize(this->width(), this->height());
}

void PyqCommentItem::setReplyComment(const QString& fromNickname, const QString& toNickname, const QString& comment) {
    QString content;
    content = content.append(QString("<font color=\"#576b95\">%1</font>").arg(fromNickname))
                  .append(QString("<font color=\"#000000\"> 回复 </font>"))
                  .append(QString("<font color=\"#576b95\">%1</font>").arg(toNickname))
                  .append(QString("<font color=\"#000000\">: </font>"))
                  .append(QString("<font color=\"#000000\">%1</font>").arg(comment));
    int oldHeight = ui->m_textBrowser->size().rheight();
    ui->m_textBrowser->setText(content);
    ui->m_textBrowser->document()->setTextWidth(ui->m_textBrowser->width());
    ui->m_textBrowser->setFixedHeight((int)(ui->m_textBrowser->document()->size().height()));
    ui->m_textBrowser->setFixedWidth((int)(ui->m_textBrowser->document()->size().width()));
    int newHeight = ui->m_textBrowser->size().rheight();
    int deltaHeight = newHeight - oldHeight;
    setFixedHeight(this->height() + deltaHeight);
    setMinimumSize(this->width(), this->height());
    setMaximumSize(this->width(), this->height());
}
