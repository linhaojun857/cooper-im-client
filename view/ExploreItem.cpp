#include "ExploreItem.hpp"

#include "ui_ExploreItem.h"

ExploreItem::ExploreItem(QWidget* parent) : QWidget(parent), ui(new Ui::ExploreItem) {
    ui->setupUi(this);
    connect(ui->m_button, &QPushButton::clicked, this, &ExploreItem::clicked);
}

ExploreItem::~ExploreItem() {
    delete ui;
}

void ExploreItem::setName(const QString& name) {
    ui->m_button->setText(name);
}
