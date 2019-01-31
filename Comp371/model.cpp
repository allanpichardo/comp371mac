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

#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "headers/OBJloader.h"
#include "headers/model.h"

Model::Model(int uniformLocation, const char* filePath) {
    this->uniformLocation = uniformLocation;
    
    loadOBJ(filePath, vertices, normals, UVs); //read the vertices from the cat object file
    
    //Generate the vertex array and vertex buffer
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0); // Unbind vao (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the vbo, keep it bound to this vao
    updateUniform();
}

void Model::updateUniform() {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
    
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    
    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &modelMatrix[0][0]);
}

void Model::scaleRelative(float ratio) {
    scale *= ratio;
    updateUniform();
}

void Model::translateXBy(float amount) {
    translation.x += amount;
    updateUniform();
}

void Model::translateYBy(float amount) {
    translation.y += amount;
    updateUniform();
}

void Model::translateZBy(float amount) {
    translation.z += amount;
    updateUniform();
}

void Model::rotateXBy(float amount) {
    rotation.x += glm::radians(amount);
    updateUniform();
}

void Model::rotateYBy(float amount) {
    rotation.y += glm::radians(amount);
    updateUniform();
}

void Model::rotateZBy(float amount) {
    rotation.z += glm::radians(amount);
    updateUniform();
}

GLuint Model::getVAO() {
    return vao;
}

GLuint Model::getVBO() {
    return vbo;
}

int Model::getVertexCount() {
    return vertices.size();
}
