FLG=-lGL -lX11 -lpthread -lXrandr -lXi -ldl
GLAD=-I glad/include
DEBUG=-g
all:
	g++ $(GLAD) `pkg-config --cflags glfw3` -o main main.cpp includes/*.cpp glad/src/glad.c `pkg-config --libs glfw3` $(FLG)

.PHONY : clean

clean:
	-rm main
