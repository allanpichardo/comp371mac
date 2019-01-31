/**
 Allan Pichardo
 #40051123
 COMP 371
 Assignment 1
 
 This abstraction loads a model from an OBJ file,
 creates a corresponding vertex buffer, and binds it
 to a new vertex array object. It also manipulates
 the model matrix through
 translation, and rotation
 */

#ifndef model_h
#define model_h

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Model {
    int uniformLocation = 0;
    GLuint vao, vbo;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale = glm::vec3(1.0f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    
    void updateUniform();
public:
    Model(int uniformLocation, const char* filePath);
    GLuint getVAO();
    GLuint getVBO();
    int getVertexCount();
    void scaleRelative(float ratio);
    void translateXBy(float amount);
    void translateYBy(float amount);
    void translateZBy(float amount);
    void rotateXBy(float amount);
    void rotateYBy(float amount);
    void rotateZBy(float amount);
};

#endif /* model_h */
