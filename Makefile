# Compiler settings
CXX=g++
CXXFLAGS=-std=c++11 -pthread -lixwebsocket -lz

# Directories
SRC_DIR=src
OBJ_DIR=obj

# Targets
TARGETS=main main1 main2 main3

# Source files
MAIN_SRC=$(SRC_DIR)/main.cpp
MAIN1_SRC=$(SRC_DIR)/main1.cpp
MAIN2_SRC=$(SRC_DIR)/main2.cpp
MAIN3_SRC=$(SRC_DIR)/main3.cpp
ARA_SRC=$(wildcard $(SRC_DIR)/ara/**/*.cpp)
ARA_HEADERS=$(wildcard $(SRC_DIR)/ara/**/*.hpp)
PROTO_SRC=$(wildcard include/*.cpp)

# Object files
MAIN_OBJ=$(OBJ_DIR)/main.o
MAIN1_OBJ=$(OBJ_DIR)/main1.o
MAIN2_OBJ=$(OBJ_DIR)/main2.o
MAIN3_OBJ=$(OBJ_DIR)/main3.o
ARA_OBJ=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(ARA_SRC))
PROTO_OBJ=$(patsubst include/%.cpp,$(OBJ_DIR)/%.o,$(PROTO_SRC))

# Build rules
all: $(TARGETS)

main: $(MAIN_OBJ) $(ARA_OBJ) $(PROTO_OBJ) $(OBJ_DIR)/socket.o $(OBJ_DIR)/ws.o $(OBJ_DIR)/gpio.o
	$(CXX) $^ -o $@ $(CXXFLAGS) 

main1: $(MAIN1_OBJ) $(ARA_OBJ) $(PROTO_OBJ) $(OBJ_DIR)/socket.o $(OBJ_DIR)/ws.o $(OBJ_DIR)/gpio.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

main2: $(MAIN2_OBJ) $(ARA_OBJ) $(PROTO_OBJ) $(OBJ_DIR)/socket.o $(OBJ_DIR)/ws.o $(OBJ_DIR)/gpio.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

main3: $(MAIN3_OBJ) $(ARA_OBJ) $(PROTO_OBJ) $(OBJ_DIR)/socket.o $(OBJ_DIR)/ws.o $(OBJ_DIR)/gpio.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.o: **/%.cpp $(ARA_HEADERS)
	@mkdir -p $(@D)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TARGETS)
