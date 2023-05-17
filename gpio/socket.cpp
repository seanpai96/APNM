// This is a modified version of socket/socket.cpp, for demo use only.
// Beside its original functions, it'll control GPIO to show its status.
// Also, some unused statements are removed.

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <ctime>

#include <vector>
#include <chrono>
#include <thread>
#include "gpio.hpp"

class Socket{
private:
    //blink the io, this will cause a 0.1s delay in current thread
    void blink(IO &io) {
        io << 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io << 0;
    }

public:
    int sock_Client,sock_Server;
    int broadcast = 1;
    struct sockaddr_in client_Addr,server_Addr;
    struct ip_mreq stMreqServer,stMreqClient;
    int port = 11115;
    std::string IP;

    int led_pin;
    GPIO_OUT gpio;

    Socket(int led_pin): led_pin{led_pin}, gpio({led_pin}) {
        ;
    }

    int setServerAndBind(std::string addr){
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
        IP = addr;
        server_Addr.sin_addr.s_addr = inet_addr(addr.c_str());

        std::cout << "Binding to " << inet_ntoa(server_Addr.sin_addr) << ":" << ntohs(server_Addr.sin_port) << std::endl;
        return 1;
    }
    int setServerLoopBack(){
        int loop = 0;
        int err = setsockopt(sock_Server, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
        if (err < 0) {
            std::cerr << "setsockopt():IP_MULTICAST_LOOP"<< strerror(errno) << std::endl;
            return 0;
        }
        return 1;
    }
    int serverSendBuffer(char node){
        char buf[4] = {0,node,0,0};
        std::cout << "sent message, content: " << buf[0]+'0' << ' ' << buf[1]+'0' << std::endl;
        sendto(sock_Server,buf,4,0,(sockaddr *)&server_Addr,sizeof(server_Addr));
        return 1;
    }
    int setClientAndBind(std::string multicastIp){
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
        client_Addr.sin_addr.s_addr = inet_addr(multicastIp.c_str());
        client_Addr.sin_port = htons(port);

        int status = bind(sock_Client, (struct sockaddr *) &client_Addr, sizeof(client_Addr));
        if (status < 0) {
            std::cerr << "bind failed"<< strerror(errno) << std::endl;
            closeClientSocket();
            return 0;
        }
        return 1;
    }
    int setClientAddGroup(std::string str){
        stMreqClient.imr_multiaddr.s_addr = inet_addr(str.c_str());
        stMreqClient.imr_interface.s_addr = htonl(INADDR_ANY);
        int err = setsockopt(sock_Client, IPPROTO_IP, IP_ADD_MEMBERSHIP, &stMreqClient, sizeof(stMreqClient));
        if (err < 0) {
            std::cerr << "setsockopt():IP_ADD_MEMBERSHIP"<< strerror(errno) << std::endl;
            return 0;
        }
        return 1;
    }
    int receiveBroadcast(char node){
        char buffer[4];
        socklen_t len = sizeof(struct sockaddr_in);
        int n =recvfrom(sock_Client, buffer, 4, 0, (struct sockaddr*)&client_Addr,&len);

        if(n == -1 || buffer[1] == node){
            if (buffer[1] == node){
                std::cout << "receive broadcast from myself which is " << buffer[1]+'0' << std::endl;
            }
            return 0;
        }else{
            std::cout << "receive broadcast from " << buffer[1]+'0' << std::endl;
            blink(gpio[led_pin]);
            return 1;
        }
    }
    void clientLeaveGroup(){
        setsockopt(sock_Client, IPPROTO_IP, IP_DROP_MEMBERSHIP, &stMreqClient, sizeof(stMreqClient));
    }
    void closeClientSocket(){
        close(sock_Client);
    }
    void closeServerSocket(){
        close(sock_Server);
    }
    void closeSocket() {
        clientLeaveGroup();
        closeClientSocket();
        closeServerSocket();
        gpio.release_ios();
    }
    void setPort(int expect){
        port = expect;
    }
};
