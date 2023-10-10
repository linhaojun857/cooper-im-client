#include "TcpClientNet.hpp"

#include <thread>

#include "mediator/INetMediator.hpp"

TcpClientNet::TcpClientNet(INetMediator* pMediator) : m_sock(INVALID_SOCKET), m_bStop(false), m_handle(nullptr) {
    m_pMediator = pMediator;
}

TcpClientNet::~TcpClientNet() = default;

bool TcpClientNet::openNet() {
    WORD version = MAKEWORD(2, 2);
    WSADATA wsaData;

    // 加载库
    int error = WSAStartup(version, &wsaData);
    if (0 != error) {
        std::cout << "WSAStartup error" << std::endl;
        return false;
    }

    // 检查版本
    if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion)) {
        std::cout << "Version error" << std::endl;
        WSACleanup();
        return false;
    } else {
        std::cout << "Version success" << std::endl;
    }

    // 创建套接字
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 连接服务器
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);
    int ret = connect(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (SOCKET_ERROR == ret) {
        std::cout << "Connect error: " << WSAGetLastError() << std::endl;
        return false;
    } else {
        std::cout << "Connect success" << std::endl;
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
    std::cout << __FUNCTION__ << std::endl;
    if (str.empty()) {
        std::cout << "TcpClientNet::send() str is empty" << std::endl;
    }
    // 再发包的内容
    if (::send(m_sock, str.c_str(), (int)str.size(), 0) == SOCKET_ERROR) {
        std::cout << "TcpClientNet::send() send err: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool TcpClientNet::sendData(char* buf, int size, long peerSock) {
    (void)peerSock;
    std::cout << __FUNCTION__ << std::endl;
    if (!buf || size <= 0) {
        std::cout << "TcpClientNet::sendData() buf is nullptr or size <= 0" << std::endl;
    }
    // 先发包的大小
    if (::send(m_sock, (char*)&size, sizeof(size), 0) == SOCKET_ERROR) {
        std::cout << "TcpClientNet::sendData() send err: " << WSAGetLastError() << std::endl;
        return false;
    }
    // 再发包的内容
    if (::send(m_sock, buf, size, 0) == SOCKET_ERROR) {
        std::cout << "TcpClientNet::sendData() send err: " << WSAGetLastError() << std::endl;
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
                    std::cout << "TcpClientNet::recvData() recv error: " << WSAGetLastError() << std::endl;
                    return;
                }
            }
            m_pMediator->dealData(buf, nOffset, static_cast<long>(m_sock));
        } else {
            std::cout << "TcpClientNet::recvData() recv error: " << WSAGetLastError() << std::endl;
            return;
        }
    }
}

unsigned TcpClientNet::recvThread(void* pParam) {
    auto pThis = reinterpret_cast<TcpClientNet*>(pParam);
    pThis->recvData();
    return 0;
}
