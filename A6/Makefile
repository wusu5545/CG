CC = g++
LFLAGS = -Wall
TARGET = part6

runme: $(TARGET).cpp 
	$(CC)  $(LFLAGS) $(TARGET).cpp -I /usr/include/qt4 -o runme -lglut -lGLU  -lGLEW -lQtGui -lQtOpenGL -lGL

clean: 
	rm ./runme
