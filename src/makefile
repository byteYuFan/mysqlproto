CC = g++
CFLAGS = -Wall -std=c++11
LDFLAGS = -lssl -lcrypto

SRC_DIR = src
HZ_PORTO_DIR = $(SRC_DIR)/hzporto
UTILS_DIR = $(SRC_DIR)/utils

SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(HZ_PORTO_DIR)/*.cpp) \
       $(wildcard $(UTILS_DIR)/*.cpp)

OBJS := $(SRCS:.cpp=.o)
TARGET = protocol

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
