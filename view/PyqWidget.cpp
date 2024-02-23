#include "PyqWidget.hpp"

#include <QJsonDocument>
#include <QScrollBar>

#include "define/IMDefine.hpp"
#include "store/IMStore.hpp"
#include "ui_PyqWidget.h"
#include "util/HttpUtil.hpp"

PyqWidget::PyqWidget(QWidget* parent) : QWidget(parent), ui(new Ui::PyqWidget) {
    ui->setupUi(this);
    setWindowTitle("朋友圈");
    setWindowIcon(QIcon(":/img/logo.ico"));

    ui->m_scrollArea->setFrameStyle(QFrame::NoFrame);
    ui->m_scrollArea->setFrameShape(QFrame::NoFrame);
    ui->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto widget = new QWidget();
    ui->m_scrollArea->setWidget(widget);
    m_pyqItemLayout = new QVBoxLayout();
    m_pyqItemLayout->setContentsMargins(0, 0, 0, 0);
    m_pyqItemLayout->setSpacing(0);
    widget->setLayout(m_pyqItemLayout);
    m_pyqItemLayout->addStretch();

    connect(ui->m_postPushButton, &QPushButton::clicked, this, &PyqWidget::handleClickPostPushButton);

    m_postPyqWidget = new PostPyqWidget();
}

PyqWidget::~PyqWidget() {
    delete ui;
}

void PyqWidget::addPyqItem(PyqItem* pyqItem) {
    m_pyqItemLayout->insertWidget(m_pyqItemLayout->count() - 1, pyqItem);
}

void PyqWidget::clearData() {
    QLayoutItem* child;
    while ((child = m_pyqItemLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    m_pyqItemLayout->addStretch();
}

void PyqWidget::flushData() {
    clearData();
    loadData();
    connect(ui->m_scrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &PyqWidget::loadMoreData);
}

void PyqWidget::loadData() {
    QJsonObject json;
    json["token"] = IMStore::getInstance()->getToken();
    json["current_page"] = m_currentPage;
    json["page_size"] = m_pageSize;
    auto ret = HttpUtil::post(HTTP_SERVER_URL "/pyq/getPyqByPage", json);
    if (ret["code"].toInt() != HTTP_SUCCESS_CODE) {
        return;
    }
    auto pyqs = ret["pyqs"].toArray();
    for (auto p : pyqs) {
        auto pyq = p.toObject();
        auto pyqItem = new PyqItem();
        pyqItem->setId(pyq["id"].toInt());
        pyqItem->setUserId(pyq["user_id"].toInt());
        pyqItem->setContent(pyq["content"].toString());
        QList<QString> imageUrls;
        auto imageUrlsArr = QJsonDocument::fromJson(pyq["image_urls"].toString().toUtf8()).array();
        for (auto img : imageUrlsArr) {
            imageUrls.append(img.toString());
        }
        pyqItem->setImages(imageUrls);
        pyqItem->setTime(pyq["timestamp"].toVariant().toLongLong());
        addPyqItem(pyqItem);
    }
}

void PyqWidget::handleClickPostPushButton() {
    m_postPyqWidget->reset();
    m_postPyqWidget->show();
}

void PyqWidget::loadMoreData(int value) {
    if (ui->m_scrollArea->verticalScrollBar()->maximum() == value) {
        m_currentPage++;
        loadData();
    }
}
