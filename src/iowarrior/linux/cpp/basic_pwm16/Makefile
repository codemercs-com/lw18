FILENAME = basic_pwm16
CC = g++
STD = -std=c++17
CFLAGS  = -Wall #-Wextra
LDFLAGS = -liowkit
C_SOURCES =


all: 
	clear
	$(CC) $(STD) $(CFLAGS) main.cpp $(C_SOURCES) $(LDFLAGS) -o $(FILENAME)

.PHONY: clean
clean:
	rm -rf *.o
