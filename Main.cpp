/**
 Allan Pichardo
 #40051123
 COMP 371
 Assignment 2
 */


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "headers/shaderloader.h"
#include "headers/camera.h"
#include "headers/model.h"
#include "headers/light.h"

const float MOVEMENT_INTERVAL = 1.0f;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;
GLFWwindow *window;

bool isMouseClicked = false;
float lastY;

Camera* camera;
Model* model;
Light* light;

int init() {
	std::cout << "Starting GLFW context, OpenGL 4.1" << std::endl;
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//WINDOW
	window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2 - Allan Pichardo", nullptr, nullptr);

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
        case GLFW_KEY_B:
            model->rotateXBy(MOVEMENT_INTERVAL);
            break;
        case GLFW_KEY_N:
            model->rotateYBy(MOVEMENT_INTERVAL);
            break;
        case GLFW_KEY_E:
            model->rotateZBy(MOVEMENT_INTERVAL);
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
    if(action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_1:
                model->toggleRed(model->getColorMask().x == 0.0f);
                break;
            case GLFW_KEY_2:
                model->toggleGreen(model->getColorMask().y == 0.0f);
                break;
            case GLFW_KEY_3:
                model->toggleBlue(model->getColorMask().z == 0.0f);
                break;
            case GLFW_KEY_4:
                model->toggleRed(true);
                model->toggleGreen(true);
                model->toggleBlue(true);
                break;
            case GLFW_KEY_5:
                light->setShadingMode(light->getShadingMode() == ShadingMode::gouraud ? ShadingMode::phrong : ShadingMode::gouraud);
                break;
            case GLFW_KEY_6:
                light->setEnabled(!light->isEnabled());
                break;
            case GLFW_KEY_M:
                light->setShadingMode(light->getShadingMode() == ShadingMode::normal ? ShadingMode::gouraud : ShadingMode::normal);
                break;
            case GLFW_KEY_G:
                model->setColorMask(glm::vec3(0.2989f, 0.587f, 0.114f));
                break;
        }
    }
    std::cout << key << std::endl;
}

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

static void clearErrors() {
    while(glGetError() != GL_NO_ERROR);
}

static bool logGlErrors() {
    while(GLenum error = glGetError()) {
        std::cout << "OpenGL Error: " << error << std::endl;
        return false;
    }
    return true;
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	if (init() != 0)
		return EXIT_FAILURE;

    glEnable(GL_DEPTH_TEST);

    //Callbacks
	glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    //Load shader
    GLuint shader = loadSHADER("../shaders/vertex.shader", "../shaders/fragment.shader");
	glUseProgram(shader);

    //Get locations for M V P uniforms
    int viewLocation = glGetUniformLocation(shader, "view");
    int projectionLocation = glGetUniformLocation(shader, "projection");

    /*
     The Model and Camera classes encapsulate all matrix transformations
     that apply either to the model or to the view. When their matrix is
     altered, the object automatically updates the corresponding uniform */
    model = new Model(shader, "../geometry/heracles.obj");

    Material material;
    material.color = glm::vec3(1, 1, 1);
    material.ambient = glm::vec3(0.25f);
    material.diffuse = glm::vec3(0.75f);
    material.specular = glm::vec3(1.0f);

    model->setMaterial(material);

    camera = new Camera(viewLocation, glm::vec3(0.0f,5.0f,50.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f));
    
    //Set projection for scene
    glm::mat4 projection = glm::perspective(90.0f, (float)(WIDTH/HEIGHT), 0.1f, 100.0f);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

    light = new Light(shader, glm::vec3(0.0f, 20.0f, 5.0f), glm::vec3(0.8f, 0.8f, 0.8f));

    glUniform3fv(glGetUniformLocation(shader, "view_position"), 1, glm::value_ptr(camera->getPosition()));

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		clearErrors();

		model->draw();

        logGlErrors();

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
    // Deallocate the model and camera pointers
    delete model;
    model = NULL;
    delete camera;
    camera = NULL;
    delete light;
    light = NULL;

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
