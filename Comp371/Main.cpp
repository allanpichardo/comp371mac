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
using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;
GLFWwindow *window;

//Camera
glm::vec3 cam_pos = glm::vec3(0,2,-5);
glm::vec3 cam_dir = glm::vec3(0, 0, 1);
glm::vec3 cam_up = glm::vec3(0,1,0);

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

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if(key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(key == GLFW_KEY_W) {
        cam_pos += cam_dir;
    }
    if(key == GLFW_KEY_S) {
        cam_pos -= cam_dir;
    }
    if(key == GLFW_KEY_A) {
        cam_pos += glm::cross(cam_up, cam_dir);
    }
    if(key == GLFW_KEY_D) {
        cam_pos -= glm::cross(cam_up, cam_dir);
    }
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	if (init() != 0)
		return EXIT_FAILURE;
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Build and compile our shader program
	// Vertex shader
	GLuint shader = loadSHADER("shaders/vertex.shader", "shaders/fragment.shader");
	glUseProgram(shader);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	loadOBJ("geometry/cube.obj", vertices, normals, UVs); //read the vertices from the cube.obj file

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	GLuint vertices_VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertices_VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    glm::mat4 modl_matrix = glm::rotate(glm::mat4(1.0f), 30.0f, glm::vec3(0,1,0));
    glm::mat4 view_matrix = glm::lookAt(cam_pos, cam_pos + cam_dir, cam_up);
    glm::mat4 proj_matrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
    
    GLuint mm_loc = glGetUniformLocation(shader, "mm");
    GLuint vm_loc = glGetUniformLocation(shader, "vm");
    GLuint pm_loc = glGetUniformLocation(shader, "pm");
    
    glUniformMatrix4fv(mm_loc, 1, GL_FALSE, &modl_matrix[0][0]);
    glUniformMatrix4fv(vm_loc, 1, GL_FALSE, &view_matrix[0][0]);
    glUniformMatrix4fv(pm_loc, 1, GL_FALSE, &proj_matrix[0][0]);

	// Game loop
    int counter = 0;
	while (!glfwWindowShouldClose(window))
	{
        counter = ++counter;
        view_matrix = glm::lookAt(cam_pos, cam_pos + cam_dir, cam_up);
        glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), (float)counter/180, glm::vec3(0,1,0));
        glm::mat4 translator = glm::translate(glm::mat4(1.0f), glm::vec3(2,0,0));
        glm::mat4 scalar = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f,1,1));
        modl_matrix = translator * rotator * scalar;
        glUniformMatrix4fv(mm_loc, 1, GL_FALSE, &modl_matrix[0][0]);
        glUniformMatrix4fv(vm_loc, 1, GL_FALSE, &view_matrix[0][0]);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
