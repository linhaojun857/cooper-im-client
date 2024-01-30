#ifndef TcpClientNet_hpp
#define TcpClientNet_hpp

#include "INet.hpp"

class TcpClientNet : public INet {
public:
    explicit TcpClientNet(INetMediator* pMediator);

    ~TcpClientNet() override;

    bool openNet(const std::string& ip, const std::string& port, int mode) override;

    void closeNet() override;

    bool send(std::string str) override;

    bool sendData(char* buf, int size) override;

    bool sendRaw(char* buf, int size) override;

protected:
    void recvData() override;

    static unsigned /*__stdcall*/ recvThread(void* pParam);

private:
    SOCKET m_sock;

    bool m_bStop;

    HANDLE m_handle;
};

#endif
