
SOURCE = main.cpp

TARGET_FLAGS = -o

TARGET = phrase

all:
	g++ $(SOURCE) $(TARGET_FLAGS) $(TARGET)

clean:
	rm $(TARGET)
