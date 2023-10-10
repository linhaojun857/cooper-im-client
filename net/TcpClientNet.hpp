#ifndef TcpClientNet_hpp
#define TcpClientNet_hpp

#include "INet.hpp"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define PING_TYPE 100
#define PONG_TYPE 200

class TcpClientNet : public INet {
public:
    explicit TcpClientNet(INetMediator* pMediator);

    ~TcpClientNet() override;

    bool openNet() override;

    void closeNet() override;

    bool send(std::string str) override;

    bool sendData(char* buf, int size, long peerSock) override;

protected:
    void recvData() override;

    static unsigned /*__stdcall*/ recvThread(void* pParam);

private:
    SOCKET m_sock;

    bool m_bStop;

    HANDLE m_handle;
};

#endif
