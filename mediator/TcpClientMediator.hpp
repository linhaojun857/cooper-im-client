#ifndef TcpClientMediator_hpp
#define TcpClientMediator_hpp

#include <QJsonObject>

#include "INetMediator.hpp"

class TcpClientMediator : public INetMediator {
    Q_OBJECT
public:
    TcpClientMediator();

    ~TcpClientMediator() override;

    bool openNet() override;

    void closeNet() override;

    bool send(std::string str) override;

    bool sendData(char* buf, int size, long peerSock) override;

    bool sendData(QJsonObject jsonObject, long peerSock) override;

    void dealData(char* buf, int size, long peerSock) override;

signals:
    void SIG_readyData(QJsonObject jsonObject, long lFrom);
};

#endif
