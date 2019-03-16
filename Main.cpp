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

bool enableShadowMap = false;
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
                //light->setShadingMode(light->getShadingMode() == ShadingMode::gouraud ? ShadingMode::phrong : ShadingMode::gouraud);
                break;
            case GLFW_KEY_6:
                //light->setEnabled(!light->isEnabled());
                break;
            case GLFW_KEY_M:
                //light->setShadingMode(light->getShadingMode() == ShadingMode::normal ? ShadingMode::gouraud : ShadingMode::normal);
                break;
            case GLFW_KEY_G:
                model->setColorMask(glm::vec3(0.2989f, 0.587f, 0.114f));
                break;
            case GLFW_KEY_F1:
                enableShadowMap = false;
                break;
            case GLFW_KEY_F2:
                enableShadowMap = true;
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
    GLuint mainShader = loadSHADER("../shaders/vertex.shader", "../shaders/fragment.shader");
    GLuint shadowShader = loadSHADER("../shaders/shadowvertex.shader", "../shaders/shadowfragment.shader");

    glUseProgram(mainShader);

    //Get locations for M V P uniforms

    /*
     The Model and Camera classes encapsulate all matrix transformations
     that apply either to the model or to the view. When their matrix is
     altered, the object automatically updates the corresponding uniform */
    model = new Model("../geometry/heracles.obj");
    model->rotateXBy(-90.0f);
    model->rotateZBy(-45.0f);
    model->translateYBy(-50.0f);
    Material material;
    material.color = glm::vec3(1, 1, 1);
    material.ambient = glm::vec3(0.25f);
    material.diffuse = glm::vec3(0.75f);
    material.specular = glm::vec3(1.0f);
    model->setMaterial(material);

    /*
     * A simple plane for the shadows to fall on
     */
    Model* plane = new Model("../geometry/plane.obj");
    plane->setMaterial(material);
    plane->setScale(glm::vec3(3.0f, 3.0f, 1.0f));
    plane->rotateXBy(-90.0f);
    plane->translateYBy(-100.0f);

    vector<Model*> models;
    models.push_back(model);
    models.push_back(plane);

    camera = new Camera((float)(WIDTH/HEIGHT), glm::vec3(0.0f,0.0f,100.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f));

    Light light1 = Light("light1", glm::vec3(10.0f, 15.0f, 5.0f), glm::vec3(0.2f, 0.05f, 0.05f));
    Light light2 = Light("light2", glm::vec3(-10.0f, 15.0f, 5.0f), glm::vec3(0.05f, 0.2f, 0.05f));
    Light light3 = Light("light3", glm::vec3(0.0f, 15.0f, 5.0f), glm::vec3(0.05f, 0.05f, 0.2f));
    Light light4 = Light("light4", glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.05f, 0.05f, 0.05f));

    Light smLight = Light("sm_light", glm::vec3(0.0f, 40.0f, 10.0f), glm::vec3(0.8f, 0.2f, 0.2f));
    smLight.makeSpotlight(glm::normalize(glm::vec3(0.0f) - smLight.getPosition()), 70.0f);
    smLight.createDepthMap();

    vector<Light> lights;
    lights.push_back(light1);
    lights.push_back(light2);
    lights.push_back(light3);
    lights.push_back(light4);
    lights.push_back(smLight);

    glm::vec3 clearColor = glm::vec3(0.0f);

    glUniform3fv(glGetUniformLocation(mainShader, "view_position"), 1, glm::value_ptr(camera->getPosition()));

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
        glfwPollEvents();
        clearErrors();

        if(enableShadowMap) {
            /*
             * Render from light's perspective
             */
            glUseProgram(shadowShader);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDisable( GL_CULL_FACE );
            smLight.renderToDepthMap(shadowShader, models);
            glEnable( GL_CULL_FACE );
        }

        /*
         * Render from camera's perspective
         */
        glUseProgram(mainShader);

        camera->updateUniform(mainShader);
        for(int i = 0; i < lights.size(); i++) {
            lights[i].updateUniform(mainShader);
        }

        glUniform1i(glGetUniformLocation(mainShader, "is_depth_map"), enableShadowMap);
        glViewport(0, 0, WIDTH, HEIGHT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, smLight.getDepthMap());
        glUniform1i(glGetUniformLocation(mainShader, "shadowMap"), 0);

		for(int i = 0; i < models.size(); i++) {
		    models[i]->updateUniform(mainShader);
		    models[i]->draw();
		}

        logGlErrors();

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
    // Deallocate the model and camera pointers
    delete model;
    model = NULL;
    delete plane;
    plane = NULL;
    delete camera;
    camera = NULL;

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
