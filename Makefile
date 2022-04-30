# Compiler
CC = g++

# Wall = enable compiler warnings
CFLAGS = -Wall

SIGNAL_SENDER_INCLUDE = signal-sender.cpp
SIGNAL_SENDER_EXECUTABLE = signal-sender

SIGNAL_HANDLER_INCLUDE = signal-handler.cpp
SIGNAL_HANDLER_EXECUTABLE = signal-handler

PIPE_INCLUDE = pipe.cpp common.cpp
PIPE_EXECUTABLE = pipe

SOCKET_INCLUDE = socket.cpp common.cpp
SOCKET_EXECUTABLE = socket

all: signal pipe socket

signal: signal_sender signal_handler

signal_sender:
	$(CC) $(CFLAGS) -o $(SIGNAL_SENDER_EXECUTABLE) $(SIGNAL_SENDER_INCLUDE)

signal_handler:
	$(CC) $(CFLAGS) -o $(SIGNAL_HANDLER_EXECUTABLE) $(SIGNAL_HANDLER_INCLUDE)

pipe:
	$(CC) $(CFLAGS) -o $(PIPE_EXECUTABLE) $(PIPE_INCLUDE)

socket:
	$(CC) $(CFLAGS) -o $(SOCKET_EXECUTABLE) $(SOCKET_INCLUDE)

clean:
	$(RM) $(SIGNAL_SENDER_EXECUTABLE) $(SIGNAL_HANDLER_EXECUTABLE) $(PIPE_EXECUTABLE) $(SOCKET_EXECUTABLE)
