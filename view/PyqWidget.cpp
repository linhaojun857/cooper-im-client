#include "PyqWidget.hpp"

#include "ui_PyqWidget.h"

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
}

PyqWidget::~PyqWidget() {
    delete ui;
}

void PyqWidget::addPyqItem(PyqItem* pyqItem) {
    m_pyqItemLayout->insertWidget(0, pyqItem);
}
