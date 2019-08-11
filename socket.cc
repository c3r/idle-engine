#include "socket.h"

void throw_exception() 
{
	throw std::runtime_error( std::strerror(errno) );
}

int Socket::CreateSocket() 
{ 
    int res = socket(kSockDomain, kSockType, kSockProtocol); 
    if (res == -1) {
		throw_exception();
	}
	
    return res;
}

void Socket::Bind() 
{
    sockaddr_in hint;
    hint.sin_family = kSockDomain;
    hint.sin_port = htons(port_);
    
    int res = inet_pton(kSockDomain, addr_.c_str(), &hint.sin_addr);
	if (res == -1) {
		throw_exception();
	}
    
    res = bind(listening_sock_, (sockaddr*) &hint, sizeof(hint));
	if (res == -1) {
		throw_exception();
	}
}

void Socket::MarkForListening() 
{ 
    int res = listen(listening_sock_, SOMAXCONN);
	if (res == -1) {
		throw_exception();
	};
}

void Socket::AcceptCall() 
{
    socklen_t client_size = sizeof(client_);
    client_sock_ = accept(listening_sock_, (sockaddr*)  &client_, &client_size);
	if (client_sock_ == -1) {
		throw_exception();
	}
        
    CloseListeningSocket();
    
    int res = getnameinfo((sockaddr*) &client_, sizeof(client_), host_, NI_MAXHOST, svc_, NI_MAXSERV, 0);
    if (res < 0) {
        const char* res = inet_ntop(AF_INET, &client_.sin_addr, host_, NI_MAXHOST);
		if (res == nullptr) {
			throw_exception();
		}
    }
}

void Socket::CloseListeningSocket() 
{
    int res = close(listening_sock_);
	if (res == -1) {
		throw_exception();
	}
    
    memset(host_, 0, NI_MAXHOST);
    memset(svc_, 0, NI_MAXSERV);
}

int Socket::ReceiveMsg() 
{
    memset(&msgbuf_, 0, MSG_BUF_SIZE);
    
    bytes_recv_ = recv(client_sock_, msgbuf_, MSG_BUF_SIZE, 0);
	if (bytes_recv_ == -1) {
		throw_exception();
	}
    
    if (bytes_recv_ == 0) { 
		return 0; 
	}
    
    return bytes_recv_;
}

std::string Socket::GetMsg() 
{ 
	return std::string(msgbuf_, 0, bytes_recv_); 
}

int Socket::SendMsg(std::string msg) 
{ 
    int res = send(client_sock_, &msg, sizeof(msg) + 1, 0);
	if (res == -1) {
		throw_exception();
	}
	
	return res;
}

int Socket::Close() 
{ 
    int res = close(client_sock_); 
	if (res == -1) {
		throw_exception();
	}
	
	return res;
}
