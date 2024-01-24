#ifndef INet_hpp
#define INet_hpp

#include <winsock2.h>

#include <iostream>
#include <set>

class INetMediator;

class INet {
public:
    INet() = default;

    virtual ~INet() = default;

    virtual bool openNet(const std::string& ip, const std::string& port) = 0;

    virtual void closeNet() = 0;

    virtual bool send(std::string str) = 0;

    virtual bool sendData(char* buf, int size) = 0;

    virtual bool sendRaw(char* buf, int size) = 0;

protected:
    virtual void recvData() = 0;

    INetMediator* m_pMediator{};
};

#endif
