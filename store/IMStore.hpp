#ifndef store_IMStore_hpp
#define store_IMStore_hpp

#include <QWebEngineView>

#include "view/ChatDialog.hpp"

class IMKernel;

class IMStore {
public:
    static IMStore* getInstance();

    IMStore();

    void setIMKernel(IMKernel* imKernel);

    ChatDialog* getChatDialog();

    void openChatPage(int id);

    void closeChatPage(int id);

    bool isOpenChatPage(int id);

private:
    IMKernel* m_imKernel = nullptr;
    ChatDialog* m_chatDialog = nullptr;
    QSet<int> m_openChatPageIds;
};

#endif
