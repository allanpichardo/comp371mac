/**
 Allan Pichardo
 #40051123
 COMP 371
 Assignment 1
 */


#include <GL/glew.h>	// include GL Extension Wrangler
#include <GLFW/glfw3.h>	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "headers/shaderloader.h"
#include "headers/camera.h"
#include "headers/model.h"

const float MOVEMENT_INTERVAL = 1.0f;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;
GLFWwindow *window;

GLuint shader;

bool isMouseClicked = false;
float lastY;

Camera* camera;
Model* model;

int init() {
	std::cout << "Starting GLFW context, OpenGL 4.1" << std::endl;
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//WINDOW
	window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 1", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}

// is called whenever a key is pressed/released via GLFW
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    switch(key) {
        case GLFW_KEY_O:
            model->scaleRelative(1.1);
            break;
        case GLFW_KEY_P:
            model->scaleRelative(0.9);
            break;
        case GLFW_KEY_I:
            model->translateYBy(MOVEMENT_INTERVAL);
            break;
        case GLFW_KEY_K:
            model->translateYBy(-MOVEMENT_INTERVAL);
            break;
        case GLFW_KEY_J:
            model->translateXBy(MOVEMENT_INTERVAL);
            break;
        case GLFW_KEY_L:
            model->translateXBy(-MOVEMENT_INTERVAL);
            break;
        case GLFW_KEY_PAGE_UP:
            model->translateZBy(MOVEMENT_INTERVAL);
            break;
        case GLFW_KEY_PAGE_DOWN:
            model->translateZBy(-MOVEMENT_INTERVAL);
            break;
        case GLFW_KEY_W:
            camera->panForward();
            break;
        case GLFW_KEY_S:
            camera->panBackward();
            break;
        case GLFW_KEY_D:
            camera->panLeft();
            break;
        case GLFW_KEY_A:
            camera->panRight();
            break;
        case GLFW_KEY_LEFT:
            camera->rotateLeft();
            break;
        case GLFW_KEY_RIGHT:
            camera->rotateRight();
            break;
        case GLFW_KEY_UP:
            camera->rotateUp();
            break;
        case GLFW_KEY_DOWN:
            camera->rotateDown();
            break;
    }
    
    std::cout << key << std::endl;
}

// Called when a mouse button is pressed.
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    isMouseClicked = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
}

// Called when mouse is moved
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(isMouseClicked) {
        if(ypos - lastY < 0) {
            camera->panBackward();
        }else {
            camera->panForward();
        }
    }
    
    lastY = ypos;
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	if (init() != 0)
		return EXIT_FAILURE;
	
    //Callbacks
	glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    //Load shader
    shader = loadSHADER("shaders/vertex.shader", "shaders/fragment.shader");
	glUseProgram(shader);

    //Get locations for M V P uniforms
    int viewLocation = glGetUniformLocation(shader, "view");
    int modelLocation = glGetUniformLocation(shader, "model");
    int projectionLocation = glGetUniformLocation(shader, "projection");
    
    /*
     The Model and Camera classes encapsulate all matrix transformations
     that apply either to the model or to the view. When their matrix is
     altered, the object automatically updates the corresponding uniform */
    model = new Model(modelLocation, "geometry/cat.obj");
    camera = new Camera(viewLocation, glm::vec3(0.0f,0.0f,-10.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,1.0f,0.0f));
    
    //Set projection for scene
    glm::mat4 projection = glm::perspective(90.0f, (float)(WIDTH/HEIGHT), 0.1f, 100.0f);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(model->getVAO());
		glDrawArrays(GL_TRIANGLES, 0, model->getVertexCount());
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
    // Deallocate the model and camera pointers
    delete model;
    model = NULL;
    delete camera;
    camera = NULL;

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
