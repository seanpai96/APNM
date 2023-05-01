#include "ws.hpp"
#include <iostream>

bool isWSServerStarted = false;

ix::WebSocketServer WSServer(8080, "0.0.0.0");

void WSBroadcast(std::string msg) {
    if (!isWSServerStarted) return;         //prevent crash when server is not started
    for (auto& connection : WSServer.getClients()) {
        connection->send(msg);
    }
}

void WSStartServer() {
    auto res = WSServer.listen();
    if (!res.first) {
        std::cout << "Error listening" << res.second << std::endl;
        return;
    }

    WSServer.disablePerMessageDeflate();
    WSServer.start();
    isWSServerStarted = true;
}

void WSStopServer() {
    WSServer.stop();
    isWSServerStarted = false;
}
