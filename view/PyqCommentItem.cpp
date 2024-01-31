#include "PyqCommentItem.hpp"

#include "ui_PyqCommentItem.h"

PyqCommentItem::PyqCommentItem(QWidget* parent) : QWidget(parent), ui(new Ui::PyqCommentItem) {
    ui->setupUi(this);
}

PyqCommentItem::~PyqCommentItem() {
    delete ui;
}
