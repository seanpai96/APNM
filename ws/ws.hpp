#ifndef _WS_WS_HPP_
#define _WS_WS_HPP_

#include "ixwebsocket/IXWebSocketServer.h"
#include <string>

extern ix::WebSocketServer WSServer;

void WSBroadcast(std::string msg);
void WSStartServer();
void WSStopServer();


#endif