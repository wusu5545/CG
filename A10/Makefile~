CC = g++
LFLAGS = -Wall
TARGET = part10

runme: $(TARGET).cpp camera.h helper.h
	$(CC)  $(LFLAGS) $(TARGET).cpp -o runme -lglut -lGLU  -lGLEW -lQtGui -lQtOpenGL -lGL -I /usr/include/qt4

clean: 
	rm ./runme
