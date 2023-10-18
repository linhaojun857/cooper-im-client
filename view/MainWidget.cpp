#include "MainWidget.hpp"

#include <FramelessHelper/Widgets/framelesswidgetshelper.h>

#include <QLayout>
#include <QMenu>
#include <QTabBar>

#include "ui_MainWidget.h"

FRAMELESSHELPER_USE_NAMESPACE

MainWidget::MainWidget(QWidget* parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    setWindowTitle("Cooper");
    setWindowIcon(QIcon(":/img/logo.ico"));
    connect(ui->m_minimizePushButton, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));
    connect(ui->m_closePushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->m_optionPushButton, &QPushButton::clicked, [this]() {
        QMenu menu;
        menu.setStyleSheet(
            "QMenu {"
            "    border: 0px;"
            "    background-color: #ffffff;"
            "}"
            "QMenu::item {"
            "    color: #000000;"
            "}"
            "QMenu::item:selected {"
            "    background-color: #ffffff;"
            "}");
        auto addFriendAndGroupAction = menu.addAction("加好友/群");
        connect(addFriendAndGroupAction, SIGNAL(triggered(bool)), this, SLOT(handleAddFriendAndGroupAction()));
        menu.addAction("设置");
        menu.exec(QCursor::pos());
    });
    FramelessWidgetsHelper::get(this)->extendsContentIntoTitleBar();
    FramelessWidgetsHelper::get(this)->setTitleBarWidget(ui->m_headerWidget);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(ui->m_optionPushButton);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(ui->m_minimizePushButton);
    FramelessWidgetsHelper::get(this)->setHitTestVisible(ui->m_closePushButton);
    m_cotPWidget = new CotPWidget(ui->m_tabWidget->widget(1));
    m_cotPWidget->setGeometry(0, 0, 300, 503);
    setMinimumWidth(300);
    setMaximumWidth(300);
    setMinimumHeight(635);
    setMaximumHeight(635);
    ui->m_tabWidget->tabBar()->hide();
    m_tabBtnMap[0] = ui->m_msgPushButton;
    m_tabBtnMap[1] = ui->m_cotPPushButton;
    m_tabBtnMap[2] = ui->m_pyqPushButton;
    connect(ui->m_msgPushButton, &QPushButton::clicked, this, [&]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_tabWidget->setCurrentIndex(0);
        m_tabCurrentIndex = 0;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
    connect(ui->m_cotPPushButton, &QPushButton::clicked, this, [&]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_tabWidget->setCurrentIndex(1);
        m_tabCurrentIndex = 1;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
    connect(ui->m_pyqPushButton, &QPushButton::clicked, this, [&]() {
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet("border: 0px;");
        ui->m_tabWidget->setCurrentIndex(2);
        m_tabCurrentIndex = 2;
        m_tabBtnMap[m_tabCurrentIndex]->setStyleSheet(
            "border: 0px;\n"
            "background-color: #d9d9d9;");
    });
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::handleAddFriendAndGroupAction() {
}
