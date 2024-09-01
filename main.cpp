#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "./Shader.hpp"
#include "./Mesh.hpp"
#include "./Model.hpp"
#include "./Camera.hpp"
#include "./Skybox.hpp"


// global variables
// these will be placed in proper renderer and window classes later 
int screenHeight = 720;
int screenWidth = 1280;
SDL_Window *graphicsApplicationWindow = nullptr;
SDL_GLContext openGLContext = nullptr;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *screenSurface;

const int FrameRate = 60;
const int FrameDelay = 1000/FrameRate;
Uint32 FrameStartTime;
int FrameUpdateDuration;
float Time = 0.0f;
float deltaT = (float)FrameDelay/1000.0f;
bool Quit = false;

glm::vec2 mouseDelta;
float mouseSensitivity = 0.075;
bool mouseIsFree = false;

// the camera is a global variable to make keyboard input more convenient
// to implement; this is simply meant for OGL testing, and will be changed later.
Camera camera;
float camFOV = 75.0f;
float camZoomRate = 1.0f;
float maxFOV = 150.0f;
float minFOV = 10.0f;

 
void startWindow(){
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
		window = SDL_CreateWindow(
			"OpenGL Renderer",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			screenWidth,
			screenHeight,
			SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL
		);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED && SDL_RENDERER_PRESENTVSYNC);
		screenSurface = SDL_GetWindowSurface(window);
	} else if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Could not load SDL Subsystems" << std::endl;
		exit(1);
	}

	if (window == nullptr){
		std::cout << "window is null" << std::endl;
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	openGLContext = SDL_GL_CreateContext(window);
	if (openGLContext == nullptr){
		std::cout << "Could not load OpenGL" << std::endl;
		exit(1);
	}

	// initializes the GLAD openGL loader
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)){
		std::cout << "GLAD failed to start" << std::endl;
		exit(1);
	}

	// sets the OpenGL screen resolution
	glViewport(0, 0, screenWidth, screenHeight);

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void getInput(){
	SDL_Event event;
	while(SDL_PollEvent(&event) != 0){
		if (event.type == SDL_QUIT){
			Quit = true;
		}

		if(event.type == SDL_MOUSEMOTION && mouseIsFree == false){
			int mouse_x, mouse_y;
			SDL_PumpEvents();
			SDL_GetMouseState(&mouse_x, &mouse_y);

			mouseDelta = glm::vec2(
				mouseSensitivity * (float)(mouse_x - screenWidth/2),
				mouseSensitivity * (float)(mouse_y - screenHeight/2)
			);
			camera.mouseLook(mouseDelta);
		}

		// press escape to free the mouse
		if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
			mouseIsFree = !mouseIsFree;
		}

		// scroll with the mouse wheel to zoom the camera in or our
		if(event.type == SDL_MOUSEWHEEL){
			if(event.wheel.y > 0){
				camFOV -= camZoomRate;
			}
			if(event.wheel.y < 0){
				camFOV += camZoomRate;
			}
			if(camFOV > maxFOV){
				camFOV = maxFOV;
			}
			if(camFOV < minFOV){
				camFOV = minFOV;
			}
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	// press q to quit
	if (state[SDL_SCANCODE_Q]){
		Quit = true;
	}

	if(mouseIsFree){
		SDL_SetRelativeMouseMode(SDL_FALSE);
	} else {
		// sets the mouse coords back to the centre of the screen; you won't see this,
		// because the mouse is hidden.
		SDL_WarpMouseInWindow(graphicsApplicationWindow, screenWidth/2, screenHeight/2);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	// hold shift to move the camera faster
	if (state[SDL_SCANCODE_LSHIFT]){
		camera.speed = 3.0f*camera.defaultSpeed;
	} else {
		camera.speed = camera.defaultSpeed;
	}

	// move the camera with the WASD keys
	if (state[SDL_SCANCODE_W]){
		camera.moveForward();
	}
	if (state[SDL_SCANCODE_S]){
		camera.moveBackward();
	}
	if (state[SDL_SCANCODE_A]){
		camera.moveLeft();
	}
	if (state[SDL_SCANCODE_D]){
		camera.moveRight();
	}
}

void closeWindow(){
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(screenSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


int main(){
	// initializes SDL, and loads OpenGL functions with Glad
	startWindow();

	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPosition = glm::vec3(0.0f, 1.0f, 0.0f);

	// initial height for the camera and the moving point light
	float initialHeight = 3.0f;
	camera.position = glm::vec3(0.0f, initialHeight, 0.0f);
	camera.speed = 0.015;
	glm::mat4 projMat;

	// a cube model; this program will render a grid of them
	Model model = Model("./Assets/Models/cube.obj");

	// the number of models in the grid along the x and z axes
	const int numX = 10;
	const int numZ = 10;

	// the model's shader
	Shader modelShader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
	modelShader.UseShader();

	// the material struct in the model's fragment shader
	modelShader.SetUniform3fv("material.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	modelShader.SetUniform3fv("material.diffuse", glm::vec3(0.07f, 0.08f, 0.2f));
	modelShader.SetUniform3fv("material.specular", glm::vec3(0.1f, 0.2f, 1.0f));
	modelShader.SetUniform1f("material.shininess", 30.0f);

	//the light struct in the model's fragment shader
	modelShader.SetUniform3fv("light.colour", lightColour);
	modelShader.SetUniform1f("light.brightness", 10.0f);
	modelShader.SetUniform1f("light.constant", 1.0f);
	modelShader.SetUniform1f("light.linear", 0.001f);
	modelShader.SetUniform1f("light.quadratic", 0.001f);

	// the lightbulb model
	Model lightModel = Model("./Assets/Models/lightbulb.obj");
	Shader lightShader("./shaders/vertex.glsl", "./shaders/lightFragment.glsl");
	lightShader.UseShader();
	lightShader.SetUniform3fv("lightColour", lightColour);

	// the skybox
	// this constructor could be simplified
	std::string path1 = "./skybox/right.jpg";
	std::string path2 = "./skybox/left.jpg";
	std::string path3 = "./skybox/top.jpg";
	std::string path4 = "./skybox/bottom.jpg";
	std::string path5 = "./skybox/front.jpg";
	std::string path6 = "./skybox/back.jpg";
	Skybox skybox(
		(char*)path1.c_str(), (char*)path2.c_str(),
		(char*)path3.c_str(), (char*)path4.c_str(),
		(char*)path5.c_str(), (char*)path6.c_str()
	);

	glEnable(GL_DEPTH_TEST);

	// it is necessary to set this depth func to render the skybox properly
	glDepthFunc(GL_LEQUAL);

	// the program / rendering loop
	while(Quit == false){
		getInput();
		float time = ((float)SDL_GetTicks()/1000.0);
		FrameStartTime = SDL_GetTicks();

		// for rendering with the default framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projMat = glm::perspective(glm::radians(camFOV), (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);

		modelShader.UseShader();
		modelShader.SetUniform3fv("cameraPosition", camera.position);
		modelShader.SetUniformMatrix4fv("camViewMatrix", camera.getViewMatrix());
		modelShader.SetUniformMatrix4fv("projectionMatrix", projMat);

		float orbitRadius = 3.0f;
		lightPosition = glm::vec3(orbitRadius*sin(time), initialHeight, orbitRadius*cos(time));
		lightShader.UseShader();
		glm::mat4 lightModelMatrix = glm::mat4(1.0f);
		lightModelMatrix = glm::translate(lightModelMatrix, lightPosition);
		lightModelMatrix = glm::scale(lightModelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
		lightShader.SetUniformMatrix4fv("modelMatrix", lightModelMatrix);
		lightShader.SetUniformMatrix4fv("camViewMatrix", camera.getViewMatrix());
		lightShader.SetUniformMatrix4fv("projectionMatrix", projMat);
		lightModel.Draw(lightShader);

		float precessionRate = 1.0f;
		glm::vec3 axis = glm::vec3(0.0f, 2.0f, 0.0f) + glm::vec3(cos(precessionRate*time), 0.0f, sin(precessionRate*time));
		axis = glm::normalize(axis);

		//manually render each object
		float scale = 0.4f;
		modelShader.UseShader();
		modelShader.SetUniform3fv("light.position", lightPosition);
		glDisable(GL_CULL_FACE);
		for(int i = 0; i < numX; i++){
			for(int j = 0; j < numZ; j++){
				float angularSpeed = 150.0f;
				glm::mat4 modelMatrix = glm::mat4(1.0f);
				float spacing = 3.0f;

				modelMatrix = glm::translate(modelMatrix, glm::vec3(spacing*(i - numX/2.0f), 0.0f, spacing*(j - numZ/2.0f)));
				modelMatrix = glm::rotate(modelMatrix, glm::radians(angularSpeed*time), axis);
				modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));

				modelShader.SetUniformMatrix4fv("modelMatrix", modelMatrix);
				model.Draw(modelShader);
			}
		}
		// render one large cube under the rest (will be used for testing shadow mapping, etc)
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		float largeScale = 10.0f;
		float spacing = 3.0f;
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -largeScale - spacing, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(largeScale, largeScale, largeScale));
		modelShader.SetUniformMatrix4fv("modelMatrix", modelMatrix);
		model.Draw(modelShader);

		// render the skybox last
		skybox.Draw(glm::mat4(glm::mat3(camera.getViewMatrix())), projMat);	

		// now render to the SDL window
		// and if the current frame took less than 16 ms to render, sleep for
		// the remainder of the alloted 16 ms frame time
		SDL_GL_SwapWindow(window);
		FrameUpdateDuration = SDL_GetTicks() - FrameStartTime;
		deltaT = (float)FrameDelay/1000.0f;
		Time += deltaT;
		if (FrameDelay > FrameUpdateDuration){
			SDL_Delay(FrameDelay - FrameUpdateDuration);
		}
	}
	// end of the program rendering loop

	closeWindow();
	return 0;
}
