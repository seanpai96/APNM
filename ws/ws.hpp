#ifndef _WS_WS_HPP_
#define _WS_WS_HPP_

#include "ixwebsocket/IXWebSocketServer.h"
#include <string>

ix::WebSocketServer WSServer(80, "0.0.0.0");

void WSBroadcast(std::string msg);
void WSStartServer();
void WSStopServer();


#endif