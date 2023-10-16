#include "IMStore.hpp"

#include <QFile>
#include <QtConcurrent/QtConcurrent>

IMStore* IMStore::getInstance() {
    static IMStore store;
    return &store;
}

IMStore::IMStore() {
    m_chatDialog = new ChatDialog();
}

void IMStore::setIMKernel(IMKernel* imKernel) {
    m_imKernel = imKernel;
}

ChatDialog* IMStore::getChatDialog() {
    return m_chatDialog;
}

void IMStore::openChatPage(int id) {
    m_openChatPageIds.insert(id);
}

void IMStore::closeChatPage(int id) {
    m_openChatPageIds.remove(id);
}

bool IMStore::isOpenChatPage(int id) {
    return m_openChatPageIds.contains(id);
}
