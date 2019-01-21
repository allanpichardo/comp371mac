//example skeleton code 2019 winter comp371
//modified from http://learnopengl.com/


#include <GL/glew.h>	// include GL Extension Wrangler
#include <GLFW/glfw3.h>	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "headers/OBJloader.h"  //include the object loader
#include "headers/shaderloader.h"

const float MOVEMENT_INTERVAL = 0.1f;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;
GLFWwindow *window;

GLuint shader;

glm::vec3 translation;
glm::vec3 rotation(0.0f, 0.0f, 0.0f);
glm::vec3 scale(1.0f);

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

void updateMVP(glm::vec3 translationVector, glm::vec3 rotationVector, glm::vec3 scaleVector) {
    int location = glGetUniformLocation(shader, "u_projection");
    if(location == -1) {
        std::cout << "Couldn't find uniform u_projection" << std::endl;
        return;
    }
    
    glm::mat4 projection = glm::perspective(90.0f, (float)(WIDTH/HEIGHT), 0.1f, 100.0f);
    
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translationVector);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationVector.x, glm::vec3(1.0f,0.0f,0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, rotationVector.y, glm::vec3(0.0f,1.0f,0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, rotationVector.z, glm::vec3(0.0f,0.0f,1.0f));
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scaleVector);
    glm::mat4 model = translationMatrix * rotationMatrix * scaleMatrix;
    
    glm::mat4 view = glm::lookAt(glm::vec3(0,0,60), glm::vec3(0,0,0), glm::vec3(0,1,0));
    
    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(location, 1, GL_FALSE, &mvp[0][0]);
}

// is called whenever a key is pressed/released via GLFW
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    switch(key) {
        case GLFW_KEY_O:
            scale *= 1.1; //scale up by 10%
            break;
        case GLFW_KEY_P:
            scale *= 0.9; //scale down by 10%
            break;
        case GLFW_KEY_I:
            //+y axis
            translation.y += MOVEMENT_INTERVAL;
            break;
        case GLFW_KEY_K:
            //-y axis
            translation.y -= MOVEMENT_INTERVAL;
            break;
        case GLFW_KEY_J:
            //+x
            translation.x += MOVEMENT_INTERVAL;
            break;
        case GLFW_KEY_L:
            //-x
            translation.x -= MOVEMENT_INTERVAL;
            break;
        case GLFW_KEY_PAGE_UP:
            //+z
            translation.z += MOVEMENT_INTERVAL;
            break;
        case GLFW_KEY_PAGE_DOWN:
            //-z
            translation.z -= MOVEMENT_INTERVAL;
            break;
    }
    
    std::cout << key << std::endl;
    updateMVP(translation, rotation, scale);
}

// Called when a mouse button is pressed.
//From https://www.glfw.org/docs/latest/input_guide.html
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT){
        if(action == GLFW_PRESS) {
            //todo: move camera into/out of scene here
        } else {
            //button released
        }
    }
}

// Called when the mouse is moved
// From https://www.glfw.org/docs/latest/input_guide.html
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    //update the camera
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	if (init() != 0)
		return EXIT_FAILURE;
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

	// Build and compile our shader program
	// Vertex shader
    shader = loadSHADER("shaders/vertex.shader", "shaders/fragment.shader");
	glUseProgram(shader);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	loadOBJ("geometry/cat.obj", vertices, normals, UVs); //read the vertices from the cat object file

    //Generate the vertex array and vertex buffer
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the VBO, keep it bound to this VAO

    updateMVP(translation, rotation, scale);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
        //updateMVP(translation, rotation, scale);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
