FLG=-lGL -lX11 -lpthread -lXrandr -lXi -ldl
GLAD=-I glad/include
DEBUG=-DNORMVECTOR_DEBUG -g3
WARNINGS=-Wall -Wextra
INCLUDES=includes/*.cpp mlp_nn/mlp_nn.c mlp_nn/matrix.c
all:
	g++ $(GLAD) `pkg-config --cflags glfw3` -o main main.cpp $(INCLUDES) glad/src/glad.c `pkg-config --libs glfw3` $(FLG)

#This enables debugging symbols and a macro for debugging normal vectors per vertex
debug:
	g++ $(DEBUG) $(GLAD) `pkg-config --cflags glfw3` -o main main.cpp $(INCLUDES) glad/src/glad.c `pkg-config --libs glfw3` $(FLG)

.PHONY : clean

clean:
	-rm main
