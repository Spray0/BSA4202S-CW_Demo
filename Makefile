TARGET = balance
CC = g++
RM = rm -rf
FLAGS = -pthread  -std=c++11 
OBJECTS = bsa.o serialport.o main.o 


all: obj out
out	:	$(OBJECTS)
	@$(CC)  -o $(TARGET)  $(OBJECTS) $(FLAGS)
obj :
	@$(CC) -c -o main.o main.cpp
	@$(CC) -c -o bsa.o ./BalanceBSA/BalanceBSA.cpp
	@$(CC) -c -o serialport.o ./BalanceBSA/serialport/serialport.cpp

clean : 
	@$(RM) $(OBJECTS) $(TARGET) data.txt