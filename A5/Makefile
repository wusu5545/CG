CC = g++
LFLAGS = -Wall
TARGET = part5

runme: $(TARGET).cpp camera.h
	$(CC)  $(LFLAGS) $(TARGET).cpp -o runme -lglut -lGLU  -lGLEW -lGL

clean: 
	rm ./runme
