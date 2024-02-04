#include "PyqItem.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

#include "ui_PyqItem.h"

PyqItem::PyqItem(QWidget* parent) : QWidget(parent), ui(new Ui::PyqItem) {
    ui->setupUi(this);
    m_imageLayout = new QGridLayout();
    m_imageLayout->setContentsMargins(0, 0, 0, 0);
    m_imageLayout->setSpacing(0);
    m_imageLayout->setAlignment(Qt::AlignTop);
    m_imageLayout->setColumnMinimumWidth(0, 134);
    m_imageLayout->setColumnMinimumWidth(1, 134);
    m_imageLayout->setColumnMinimumWidth(2, 134);
    ui->m_imageWidget->setLayout(m_imageLayout);

    m_commentLayout = new QVBoxLayout();
    m_commentLayout->setContentsMargins(0, 0, 0, 0);
    m_commentLayout->setSpacing(0);
    ui->m_commentInnerWidget->setLayout(m_commentLayout);
    m_commentLayout->addStretch();

    connect(ui->m_deletePushButton, &QPushButton::clicked, this, &PyqItem::handleClickDeletePushButton);
    connect(ui->m_likePushButton, &QPushButton::clicked, this, &PyqItem::handleClickLikePushButton);
    connect(ui->m_commentPushButton, &QPushButton::clicked, this, &PyqItem::handleClickCommentPushButton);
}

PyqItem::~PyqItem() {
    delete ui;
    for (auto label : m_imageLabels) {
        delete label;
    }
    delete m_imageLayout;
    for (int i = 0; i < m_commentLayout->count(); ++i) {
        delete m_commentLayout->itemAt(i)->widget();
    }
    delete m_commentLayout;
}

void PyqItem::setAvatar(const QString& avatarUrl) {
    std::ignore = QtConcurrent::run([=]() {
        auto manager = new QNetworkAccessManager();
        QNetworkRequest request(avatarUrl);
        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data);
            ui->m_avatarLabel->setPixmap(pixmap);
        } else {
            qDebug() << "load failed: " << reply->errorString();
        }
    });
}

void PyqItem::setNickname(const QString& nickname) {
    ui->m_nicknameLabel->setText(nickname);
}

void PyqItem::setContent(const QString& pyqContent) {
    int oldHeight = ui->m_contentTextBrowser->size().rheight();
    ui->m_contentTextBrowser->setText(pyqContent);
    ui->m_contentTextBrowser->document()->setTextWidth(ui->m_contentTextBrowser->width());
    ui->m_contentTextBrowser->setFixedHeight((int)(ui->m_contentTextBrowser->document()->size().height()));
    ui->m_contentTextBrowser->setFixedWidth((int)(ui->m_contentTextBrowser->document()->size().width()));
    int newHeight = ui->m_contentTextBrowser->size().rheight();
    int deltaHeight = newHeight - oldHeight;
    setFixedHeight(this->height() + deltaHeight);
    ui->m_imageWidget->move(ui->m_imageWidget->x(), ui->m_imageWidget->y() + deltaHeight);
    ui->m_timeLabel->move(ui->m_timeLabel->x(), ui->m_timeLabel->y() + deltaHeight);
    ui->m_deletePushButton->move(ui->m_deletePushButton->x(), ui->m_deletePushButton->y() + deltaHeight);
    ui->m_likePushButton->move(ui->m_likePushButton->x(), ui->m_likePushButton->y() + deltaHeight);
    ui->m_commentPushButton->move(ui->m_commentPushButton->x(), ui->m_commentPushButton->y() + deltaHeight);
    ui->m_commentWidget->move(ui->m_commentWidget->x(), ui->m_commentWidget->y() + deltaHeight);
}

void PyqItem::setNoImage() {
    ui->m_imageWidget->hide();
    int deltaHeight = -ui->m_imageWidget->height() - 10;
    changeLayoutWhenSetImages(deltaHeight);
}

void PyqItem::setImages(const QList<QString>& imageUrls) {
    qsizetype size = imageUrls.size();
    if (size <= 3) {
        setImages3(imageUrls);
    } else if (size <= 4) {
        setImages4(imageUrls);
    } else if (size <= 6) {
        setImages6(imageUrls);
    } else if (size <= 9) {
        setImages9(imageUrls);
    }
}

void PyqItem::setTime(long long int timestamp) {
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime messageTime = QDateTime::fromSecsSinceEpoch(timestamp);
    QString ret;
    if (currentTime.date() == messageTime.date()) {
        ret = messageTime.toString("hh:mm");
    } else {
        QDateTime yesterday = currentTime.addDays(-1);
        if (yesterday.date() == messageTime.date()) {
            ret = "昨天 " + messageTime.toString("hh:mm");
        } else if (currentTime.date().year() == messageTime.date().year()) {
            ret = messageTime.toString("MM-dd");
        } else {
            ret = messageTime.toString("yyyy-MM-dd");
        }
    }
    ui->m_timeLabel->setText(ret);
}

void PyqItem::setNoLike(bool hasComment) {
    if (!hasComment || m_commentCount == 0) {
        ui->m_commentWidget->hide();
        int deltaHeight = -ui->m_commentWidget->height() - 6;
        setFixedHeight(this->height() + deltaHeight);
        setMinimumSize(this->width(), this->height());
        setMaximumSize(this->width(), this->height());
    } else {
        ui->m_commentWidget->setFixedHeight(0);
        ui->m_commentInnerWidget->move(ui->m_commentInnerWidget->x(),
                                       ui->m_commentInnerWidget->y() - ui->m_likeListTextBrowser->height());
    }
}

void PyqItem::setNoComment() {
    ui->m_likeListTextBrowser->setStyleSheet("border-bottom: 0px;");
}

void PyqItem::addPyqCommentItem(PyqCommentItem* pyqCommentItem) {
    pyqCommentItem->setParent(ui->m_commentInnerWidget);
    m_commentLayout->insertWidget(m_commentLayout->count() - 1, pyqCommentItem);
    int deltaHeight = pyqCommentItem->height();
    changeLayoutWhenSetComments(deltaHeight);
    m_commentCount++;
}

void PyqItem::handleClickDeletePushButton() {
}

void PyqItem::handleClickLikePushButton() {
}

void PyqItem::handleClickCommentPushButton() {
}

void PyqItem::setImages3(const QList<QString>& imageUrls) {
    for (int i = 0; i < imageUrls.size(); ++i) {
        auto label = new QLabel();
        label->setFixedSize(134, 134);
        std::ignore = QtConcurrent::run([=]() {
            auto manager = new QNetworkAccessManager();
            QNetworkRequest request(imageUrls[i]);
            QNetworkReply* reply = manager->get(request);
            QEventLoop loop;
            QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray data = reply->readAll();
                QPixmap pixmap;
                pixmap.loadFromData(data);
                pixmap = pixmap.scaled(label->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
                label->setPixmap(pixmap);
            } else {
                qDebug() << "load failed: " << reply->errorString();
            }
        });
        m_imageLayout->addWidget(label, i / 3, i % 3);
        m_imageLabels.append(label);
    }
}

void PyqItem::setImages4(const QList<QString>& imageUrls) {
    int oldHeight = ui->m_imageWidget->height();
    ui->m_imageWidget->setFixedHeight(oldHeight * 2);
    int newHeight = ui->m_imageWidget->height();
    int deltaHeight = newHeight - oldHeight;
    changeLayoutWhenSetImages(deltaHeight);
    int index = 0;
    for (const auto& imageUrl : imageUrls) {
        auto label = new QLabel();
        label->setFixedSize(134, 134);
        std::ignore = QtConcurrent::run([=]() {
            auto manager = new QNetworkAccessManager();
            QNetworkRequest request(imageUrl);
            QNetworkReply* reply = manager->get(request);
            QEventLoop loop;
            QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray data = reply->readAll();
                QPixmap pixmap;
                pixmap.loadFromData(data);
                pixmap = pixmap.scaled(label->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
                label->setPixmap(pixmap);
            } else {
                qDebug() << "load failed: " << reply->errorString();
            }
        });
        m_imageLayout->addWidget(label, index / 3, index % 3);
        index++;
        if (index == 2) {
            index++;
        }
        m_imageLabels.append(label);
    }
}

void PyqItem::setImages6(const QList<QString>& imageUrls) {
    int oldHeight = ui->m_imageWidget->height();
    ui->m_imageWidget->setFixedHeight(oldHeight * 2);
    int newHeight = ui->m_imageWidget->height();
    int deltaHeight = newHeight - oldHeight;
    changeLayoutWhenSetImages(deltaHeight);
    for (int i = 0; i < imageUrls.size(); ++i) {
        auto label = new QLabel();
        label->setFixedSize(134, 134);
        std::ignore = QtConcurrent::run([=]() {
            auto manager = new QNetworkAccessManager();
            QNetworkRequest request(imageUrls[i]);
            QNetworkReply* reply = manager->get(request);
            QEventLoop loop;
            QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray data = reply->readAll();
                QPixmap pixmap;
                pixmap.loadFromData(data);
                pixmap = pixmap.scaled(label->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
                label->setPixmap(pixmap);
            } else {
                qDebug() << "load failed: " << reply->errorString();
            }
        });
        m_imageLayout->addWidget(label, i / 3, i % 3);
        m_imageLabels.append(label);
    }
}

void PyqItem::setImages9(const QList<QString>& imageUrls) {
    int oldHeight = ui->m_imageWidget->height();
    ui->m_imageWidget->setFixedHeight(oldHeight * 3);
    int newHeight = ui->m_imageWidget->height();
    int deltaHeight = newHeight - oldHeight;
    changeLayoutWhenSetImages(deltaHeight);
    for (int i = 0; i < imageUrls.size(); ++i) {
        auto label = new QLabel();
        label->setFixedSize(134, 134);
        std::ignore = QtConcurrent::run([=]() {
            auto manager = new QNetworkAccessManager();
            QNetworkRequest request(imageUrls[i]);
            QNetworkReply* reply = manager->get(request);
            QEventLoop loop;
            QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray data = reply->readAll();
                QPixmap pixmap;
                pixmap.loadFromData(data);
                pixmap = pixmap.scaled(label->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
                label->setPixmap(pixmap);
            } else {
                qDebug() << "load failed: " << reply->errorString();
            }
        });
        m_imageLayout->addWidget(label, i / 3, i % 3);
        m_imageLabels.append(label);
    }
}

void PyqItem::changeLayoutWhenSetImages(int deltaHeight) {
    setFixedHeight(this->height() + deltaHeight);
    setMinimumSize(this->width(), this->height());
    setMaximumSize(this->width(), this->height());
    ui->m_timeLabel->move(ui->m_timeLabel->x(), ui->m_timeLabel->y() + deltaHeight);
    ui->m_deletePushButton->move(ui->m_deletePushButton->x(), ui->m_deletePushButton->y() + deltaHeight);
    ui->m_likePushButton->move(ui->m_likePushButton->x(), ui->m_likePushButton->y() + deltaHeight);
    ui->m_commentPushButton->move(ui->m_commentPushButton->x(), ui->m_commentPushButton->y() + deltaHeight);
    ui->m_commentWidget->move(ui->m_commentWidget->x(), ui->m_commentWidget->y() + deltaHeight);
}

void PyqItem::changeLayoutWhenSetComments(int deltaHeight) {
    setFixedHeight(this->height() + deltaHeight);
    setMinimumSize(this->width(), this->height());
    setMaximumSize(this->width(), this->height());
    ui->m_commentWidget->setFixedHeight(ui->m_commentWidget->height() + deltaHeight);
    ui->m_commentInnerWidget->setFixedHeight(ui->m_commentInnerWidget->height() + deltaHeight);
}
