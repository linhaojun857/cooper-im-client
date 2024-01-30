#ifndef TcpClientMediator_hpp
#define TcpClientMediator_hpp

#include <QJsonObject>

#include "INetMediator.hpp"

class TcpClientMediator : public INetMediator {
    Q_OBJECT
public:
    TcpClientMediator();

    ~TcpClientMediator() override;

    bool openNet(const std::string& ip, const std::string& port, int mode) override;

    void closeNet() override;

    bool send(std::string str) override;

    bool sendData(char* buf, int size) override;

    bool sendData(QJsonObject jsonObject) override;

    bool sendRaw(char* buf, int size) override;

    void dealData(char* buf, int size) override;

signals:
    void SIG_readyBusinessData(const QJsonObject& jsonObject);

    void SIG_readyMediaData(char* buf, int size);

public:
    int m_mode = 1;
};

#endif
