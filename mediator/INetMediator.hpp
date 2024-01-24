#ifndef INetMediator_hpp
#define INetMediator_hpp

#include <QJsonObject>
#include <QObject>

class INet;

class INetMediator : public QObject {
    Q_OBJECT
public:
    INetMediator() = default;

    ~INetMediator() override = default;

    virtual bool openNet(const std::string& ip, const std::string& port) = 0;

    virtual void closeNet() = 0;

    virtual bool send(std::string str) = 0;

    virtual bool sendData(char* buf, int size) = 0;

    virtual bool sendData(QJsonObject jsonObject) = 0;

    virtual bool sendRaw(char* buf, int size) = 0;

    virtual void dealData(char* buf, int size) = 0;

protected:
    INet* m_pNet{};
};

#endif
