CC = g++
LFLAGS = -Wall
TARGET = part7

runme: $(TARGET).cpp offLoader.h
	$(CC)  $(LFLAGS) $(TARGET).cpp -I /usr/include/qt4 -o runme -lglut -lGLU  -lGLEW -lQtGui -lQtOpenGL

clean: 
	rm ./runme
