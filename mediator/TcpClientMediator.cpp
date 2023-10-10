#include "TcpClientMediator.hpp"

#include <QJsonDocument>

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

bool TcpClientMediator::openNet() {
    return m_pNet->openNet();
}

void TcpClientMediator::closeNet() {
    m_pNet->closeNet();
}

bool TcpClientMediator::send(std::string str) {
    return m_pNet->send(str);
}

bool TcpClientMediator::sendData(char* buf, int size, long peerSock) {
    return m_pNet->sendData(buf, size, peerSock);
}

bool TcpClientMediator::sendData(QJsonObject jsonObject, long peerSock) {
    QJsonDocument jsonDocument(jsonObject);
    QByteArray byteArray = jsonDocument.toJson(QJsonDocument::Compact);
    return m_pNet->sendData(byteArray.data(), (int)byteArray.size(), 0);
}

void TcpClientMediator::dealData(char* buf, int size, long peerSock) {
    qDebug() << __FUNCTION__;
    QByteArray byteArray(buf, size);
    QJsonDocument jsonDocument = QJsonDocument::fromJson(byteArray);
    QJsonObject jsonObject = jsonDocument.object();
    if (jsonObject["type"].toInt() == PING_TYPE) {
        jsonObject["type"] = PONG_TYPE;
        sendData(jsonObject, peerSock);
        return;
    }
    emit SIG_readyData(jsonObject, peerSock);
}
