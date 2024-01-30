#include "TcpClientNet.hpp"

#include <WS2tcpip.h>

#include <QDebug>
#include <thread>

#include "define/IMDefine.hpp"
#include "mediator/INetMediator.hpp"

TcpClientNet::TcpClientNet(INetMediator* pMediator) : m_sock(INVALID_SOCKET), m_bStop(false), m_handle(nullptr) {
    m_pMediator = pMediator;
}

TcpClientNet::~TcpClientNet() = default;

bool TcpClientNet::openNet(const std::string& ip, const std::string& port) {
    WORD version = MAKEWORD(2, 2);
    WSADATA wsaData;

    // 加载库
    int error = WSAStartup(version, &wsaData);
    if (0 != error) {
        qDebug() << "WSAStartup error";
        return false;
    }

    // 检查版本
    if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion)) {
        qDebug() << "Version error";
        WSACleanup();
        return false;
    } else {
        qDebug() << "Version success";
    }

    // 创建套接字
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int nSendBuf = 8 * 1024 * 1024;
    setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBuf, sizeof(int));

    int nRecvBuf = 8 * 1024 * 1024;
    setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));

    // 连接服务器
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
    serverAddr.sin_port = htons(std::stoi(port));
    int ret = connect(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (SOCKET_ERROR == ret) {
        qDebug() << "Connect error: " << WSAGetLastError();
        return false;
    } else {
        qDebug() << "Connect success";
    }

    // 启动接收线程
    m_handle = (HANDLE)_beginthreadex(nullptr, 0, recvThread, this, 0, nullptr);
    return true;
}

void TcpClientNet::closeNet() {
    m_bStop = true;
    if (m_handle) {
        if (WAIT_TIMEOUT == WaitForSingleObject(m_handle, 100)) {
            TerminateThread(m_handle, -1);
        }
        CloseHandle(m_handle);
        m_handle = nullptr;
    }
    if (m_sock && m_sock != INVALID_SOCKET) {
        closesocket(m_sock);
        m_sock = INVALID_SOCKET;
    }
    WSACleanup();
}

bool TcpClientNet::send(std::string str) {
    qDebug() << __FUNCTION__;
    if (str.empty()) {
        qDebug() << "TcpClientNet::send() str is empty";
    }
    // 再发包的内容
    if (::send(m_sock, str.c_str(), (int)str.size(), 0) == SOCKET_ERROR) {
        qDebug() << "TcpClientNet::send() send err: " << WSAGetLastError();
        return false;
    }
    return true;
}

bool TcpClientNet::sendData(char* buf, int size) {
    qDebug() << __FUNCTION__;
    if (!buf || size <= 0) {
        qDebug() << "TcpClientNet::sendData() buf is nullptr or size <= 0";
    }
    // 先发包的大小
    if (::send(m_sock, (char*)&size, sizeof(size), 0) == SOCKET_ERROR) {
        qDebug() << "TcpClientNet::sendData() send err: " << WSAGetLastError();
        return false;
    }
    // 再发包的内容
    if (::send(m_sock, buf, size, 0) == SOCKET_ERROR) {
        qDebug() << "TcpClientNet::sendData() send err: " << WSAGetLastError();
        return false;
    }
    return true;
}

bool TcpClientNet::sendRaw(char* buf, int size) {
    if (!buf || size <= 0) {
        qDebug() << "TcpClientNet::sendData() buf is nullptr or size <= 0";
    }
    if (::send(m_sock, buf, size, 0) == SOCKET_ERROR) {
        qDebug() << "TcpClientNet::sendData() send err: " << WSAGetLastError();
        return false;
    }
    return true;
}

void TcpClientNet::recvData() {
    int nPackSize = 0;
    int nRecvNum;
    int nOffset;
    while (!m_bStop) {
        // 接受包的大小
        nRecvNum = recv(m_sock, (char*)&nPackSize, sizeof(int), 0);
        if (nRecvNum > 0) {
            char* buf = new char[nPackSize];
            nOffset = 0;
            // 接收数据
            while (nPackSize > 0) {
                nRecvNum = recv(m_sock, buf + nOffset, nPackSize, 0);
                if (nRecvNum > 0) {
                    nOffset += nRecvNum;
                    nPackSize -= nRecvNum;
                } else {
                    qDebug() << "TcpClientNet::recvData() recv error: " << WSAGetLastError();
                    return;
                }
            }
            m_pMediator->dealData(buf, nOffset);
        } else {
            qDebug() << "TcpClientNet::recvData() recv error: " << WSAGetLastError();
            return;
        }
    }
}

unsigned TcpClientNet::recvThread(void* pParam) {
    auto pThis = reinterpret_cast<TcpClientNet*>(pParam);
    pThis->recvData();
    return 0;
}
