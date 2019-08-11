#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <string.h>

# define MSG_BUF_SIZE 4096
# define DEFAULT_LISTENING_PORT 54000

class Socket {
    private:
        const int kSockDomain = AF_INET;
        const int kSockType = SOCK_STREAM; 
        const int kSockProtocol = 0;

        std::string addr_;
        int port_;
        int listening_sock_;
        int client_sock_;
        char host_[NI_MAXHOST];
        char svc_[NI_MAXSERV];
        sockaddr_in client_;
        char msgbuf_[MSG_BUF_SIZE];
        int bytes_recv_;
        int CreateSocket();
        void MarkForListening();
        void AcceptCall();
        void Bind();
        void CloseListeningSocket();

    public:

        Socket(std::string addr, uint16_t port): addr_(addr), port_(port) {
            listening_sock_ = CreateSocket(); 
            Bind();
            MarkForListening();
            AcceptCall();
        }
        ~Socket() { Close(); };

        int ReceiveMsg();
        std::string GetMsg();
        int SendMsg(std::string msg);
        int Close();
};