# this makefile is a WIP, and of course it is not as simple or as organized as it could be.
#

program: main.o glad.o Shader.o Mesh.o Model.o Camera.o Skybox.o
	g++ main.o glad.o Shader.o Mesh.o Model.o Camera.o Skybox.o -lSDL2 -ldl -lassimp -o OpenGLrenderer

main.o: main.cpp
	g++ -c main.cpp -I./include/

glad.o: ./src/glad.c ./include/glad/glad.h
	g++ -c ./src/glad.c -I./include/

Shader.o: Shader.cpp Shader.hpp
	g++ -c Shader.cpp -I./include/

Mesh.o: Mesh.cpp Mesh.hpp
	g++ -c Mesh.cpp -I./include/

Model.o: Model.cpp Model.hpp
	g++ -c Model.cpp -I./include/

Camera.o: Camera.cpp Camera.hpp
	g++ -c Camera.cpp -I./include

Vector2.o: Vector2.cpp Vector2.hpp
	g++ -c Vector2.cpp

Vector3.o: Vector3.cpp Vector3.hpp
	g++ -c Vector3.cpp

Quaternion.o: Quaternion.cpp Quaternion.hpp
	g++ -c Quaternion.cpp

Skybox.o: Skybox.cpp Skybox.hpp
	g++ -c Skybox.cpp -I./include/

clean:
	rm *.o OpenGLrenderer
