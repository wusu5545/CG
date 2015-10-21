CC = g++
LFLAGS = -Wall
TARGET = part9

runme: $(TARGET).cpp
	$(CC)  $(LFLAGS) $(TARGET).cpp -I /usr/include/qt4 -o runme -lglut -lGLU  -lGLEW -lQtGui -lQtOpenGL

clean: 
	rm ./runme
