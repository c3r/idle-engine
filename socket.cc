#include "socket.h"

int Socket::CreateSocket() { 
    return socket(kSockDomain, kSockType, kSockProtocol);  // TODO: error handling
}

void Socket::Bind() {
    sockaddr_in hint;
    hint.sin_family = kSockDomain;
    hint.sin_port = htons(port_);
    inet_pton(kSockDomain, addr_.c_str(), &hint.sin_addr);
    bind(listening_sock_, (sockaddr*) &hint, sizeof(hint));
}

void Socket::MarkForListening() { 
    listen(listening_sock_, SOMAXCONN); // TODO: error handling
}

void Socket::AcceptCall() {
    socklen_t client_size = sizeof(client_);
    client_sock_ = accept(listening_sock_, (sockaddr*)  &client_, &client_size);
    CloseListeningSocket();
    int result = getnameinfo((sockaddr*) &client_, sizeof(client_), host_, NI_MAXHOST, svc_, NI_MAXSERV, 0);
    if (result < 0) {
        inet_ntop(AF_INET, &client_.sin_addr, host_, NI_MAXHOST);
    }
}

void Socket::CloseListeningSocket() {
    close(listening_sock_);
    memset(host_, 0, NI_MAXHOST);
    memset(svc_, 0, NI_MAXSERV);
}

int Socket::ReceiveMsg() {
    memset(&msgbuf_, 0, MSG_BUF_SIZE);
    bytes_recv_ = recv(client_sock_, msgbuf_, MSG_BUF_SIZE, 0);
    switch(bytes_recv_) {
        case -1: return -1;
        case  0: return  0;
    }
    return bytes_recv_;
}

std::string Socket::GetMsg() { return std::string(msgbuf_, 0, bytes_recv_); }

int Socket::SendMsg(std::string msg) { 

    return send(client_sock_, &msg, sizeof(msg) + 1, 0); // TODO: error handling 
}

int Socket::Close() { 
    return close(client_sock_); // TODO: error handling
}