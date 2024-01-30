#include "TcpClientMediator.hpp"

#include <QJsonDocument>

#include "define/IMDefine.hpp"
#include "net/TcpClientNet.hpp"

TcpClientMediator::TcpClientMediator() {
    m_pNet = new TcpClientNet(this);
}

TcpClientMediator::~TcpClientMediator() {
    if (m_pNet != nullptr) {
        delete m_pNet;
        m_pNet = nullptr;
    }
}

bool TcpClientMediator::openNet(const std::string& ip, const std::string& port, int mode) {
    m_mode = mode;
    return m_pNet->openNet(ip, port, mode);
}

void TcpClientMediator::closeNet() {
    m_pNet->closeNet();
}

bool TcpClientMediator::send(std::string str) {
    return m_pNet->send(str);
}

bool TcpClientMediator::sendData(char* buf, int size) {
    return m_pNet->sendData(buf, size);
}

bool TcpClientMediator::sendData(QJsonObject jsonObject) {
    QJsonDocument jsonDocument(jsonObject);
    QByteArray byteArray = jsonDocument.toJson(QJsonDocument::Compact);
    return m_pNet->sendData(byteArray.data(), (int)byteArray.size());
}

bool TcpClientMediator::sendRaw(char* buf, int size) {
    return m_pNet->sendRaw(buf, size);
}

void TcpClientMediator::dealData(char* buf, int size) {
    if (m_mode == 1) {
        QByteArray byteArray(buf, size);
        QJsonDocument jsonDocument = QJsonDocument::fromJson(byteArray);
        QJsonObject jsonObject = jsonDocument.object();
        if (jsonObject["type"].toInt() == PING_TYPE) {
            jsonObject["type"] = PONG_TYPE;
            sendData(jsonObject);
            return;
        }
        emit SIG_readyBusinessData(jsonObject);
    } else if (m_mode == 2) {
        emit SIG_readyMediaData(buf, size);
    }
}
