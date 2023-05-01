all:
	g++ -std="c++11" ws/ws.cpp ws/ws.hpp include/prototype.h include/prototype.cpp include/nlohmann/json.hpp socket/socket.cpp src/main.cpp src/ara/com/*.hpp src/ara/com/*.cpp src/ara/core/*.h src/ara/nm/*.hpp src/ara/nm/*.cpp -lixwebsocket -lpthread -lz -o main
