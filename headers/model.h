/**
 Allan Pichardo
 #40051123
 COMP 371
 Assignment 2
 
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
#include "material.h"

class Model {
    int shader;
    Material material;
    GLuint vao, vertexVbo, normalsVbo, ebo;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;
    glm::vec3 translation;
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::vec3 colorMask = glm::vec3(1.0f);
    
    void updateUniform();
    void updateColors();
public:
    Model(int uniformLocation, const char* filePath);
    GLuint getVAO();
    GLuint getVertexVBO();
    GLuint getNormalsVBO();
    GLuint getEBO();
    int getVertexCount();
    int getIndexCount();
    void scaleRelative(float ratio);
    void translateXBy(float amount);
    void translateYBy(float amount);
    void translateZBy(float amount);
    void rotateXBy(float amount);
    void rotateYBy(float amount);
    void rotateZBy(float amount);
    void draw();
    void setColorMask(glm::vec3 colorMask);
    glm::vec3 getColorMask() {return colorMask;};
    void toggleRed(bool);
    void toggleGreen(bool);
    void toggleBlue(bool);
    void setMaterial(Material material);
};

#endif /* model_h */
