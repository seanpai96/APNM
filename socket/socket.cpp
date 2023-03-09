#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include <assert.h>
#include <string.h>
#include <ctime>


//todo : call broacast addr
//todo : package fix
//todo : test

class Socket{
    int sock_Client,sock_Server;
    int broadcast = 1;
    struct sockaddr_in client_Addr,server_Addr;
    int port = 11115;
    int setServerAndBind(){
        sock_Server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock_Server < 0) {
            std::cerr << "socket creation failed" << std::endl;
            return 0;
        }
        if(setsockopt(sock_Server, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcast, sizeof(broadcast)) < 0)
        {
            std::cerr << "socket creation failed" << std::endl;
            return 0;
        }
        memset(&server_Addr, 0, sizeof(server_Addr));
        server_Addr.sin_family = AF_INET;
        server_Addr.sin_port = htons(port);
        server_Addr.sin_addr.s_addr = INADDR_BROADCAST ;//need to set addr
        //std::cout << "Binding to " << inet_ntoa(server_Addr.sin_addr) << ":" << ntohs(server_Addr.sin_port) << std::endl;
        return 1;
    }
    int serverSendBuffer(){
        char buf[1000];
        strcpy(buf, "twireshark\n");
        unsigned slen=sizeof(sockaddr);
        sendto(sock_Server,buf,strlen(buf)-1,0,(sockaddr *)&server_Addr,sizeof(server_Addr));
        return 1;
    }
    int setClientAndBind(){
        sock_Client = socket(AF_INET, SOCK_DGRAM, 0);
        fcntl(sock_Client, F_SETFL, O_NONBLOCK);
        if (sock_Client < 0) {
            std::cerr << "socket creation failed" << std::endl;
            return 0;
        }
        if(setsockopt(sock_Client, SOL_SOCKET, SO_REUSEADDR, (const char*)&broadcast, sizeof(broadcast)) < 0)
        {
            std::cerr << "socket set failed" << std::endl;
            return 0;
        }
        sockaddr_in client_Addr;
        memset(&client_Addr, 0, sizeof(client_Addr));
        client_Addr.sin_family = AF_INET;
        client_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
        client_Addr.sin_port = htons(port);
        //std::cout << "Binding to " << inet_ntoa(client_Addr.sin_addr) << ":" << ntohs(client_Addr.sin_port) << std::endl;

        if (bind(sock_Client, (struct sockaddr *) &client_Addr, sizeof(client_Addr)) < 0) { // 绑定套接字
            std::cerr << "bind failed"<< strerror(errno) << std::endl;
            closeClientSocket();
            return 0;
        }
        return 1;
    }
    int receiveBroadcast(){
        char buffer[1024];
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int n = recv( sock_Client,buffer, 1024,0);
        if(n == -1){
            return 0;
        }else{
            return 1;
        }
    }
    void closeClientSocket(){
        close(sock_Client);
    }
    void closeServerSocket(){
        close(sock_Server);
    }
    void setPort(int expect){
        port = expect;
    }
};