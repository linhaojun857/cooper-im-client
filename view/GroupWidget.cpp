#include "GroupWidget.hpp"

#include <QPushButton>

#include "mock/Mock.hpp"
#include "ui_GroupWidget.h"

GroupWidget::GroupWidget(QWidget* parent) : QWidget(parent), ui(new Ui::GroupWidget) {
    // mock
    Mock::groupWidget = this;
    ui->setupUi(this);
    ui->m_scrollArea->setFrameStyle(QFrame::NoFrame);
    ui->m_scrollArea->setFrameShape(QFrame::NoFrame);
    ui->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto widget = new QWidget();
    ui->m_scrollArea->setWidget(widget);
    m_groupItemsLayout = new QVBoxLayout();
    m_groupItemsLayout->setContentsMargins(0, 0, 0, 0);
    m_groupItemsLayout->setSpacing(0);
    widget->setLayout(m_groupItemsLayout);
    m_groupItemsLayout->addStretch();
}

GroupWidget::~GroupWidget() {
    delete ui;
}

void GroupWidget::addGroupItem(GroupItem* groupItem) {
    qDebug() << "GroupWidget::addGroupItem";
    m_groupItems.insert(groupItem->getId(), groupItem);
    m_groupItemsLayout->insertWidget(m_groupItemsLayout->count() - 1, groupItem);
}
