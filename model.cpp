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

#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <model.h>

#include "headers/OBJloader.h"
#include "headers/model.h"

Model::Model(const char* filePath) {
    loadOBJ(filePath, indices, vertices, normals, UVs);

    //Generate the vertex array and vertex buffer
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &normalsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0); // Unbind vao (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the vbo, keep it bound to this vao
}

void Model::updateUniform(int shader) {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
    
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    
    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &modelMatrix[0][0]);

    glm::vec3 masked = material.color * colorMask;
    glUniform3fv(glGetUniformLocation(shader, "material.color"), 1, &masked[0]);
    glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, &material.ambient[0]);
    glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, &material.diffuse[0]);
    glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, &material.specular[0]);
    glUniform1f(glGetUniformLocation(shader, "material.shininess"), material.shininess);
}

void Model::scaleRelative(float ratio) {
    scale *= ratio;
}

void Model::translateXBy(float amount) {
    translation.x += amount;
}

void Model::translateYBy(float amount) {
    translation.y += amount;
}

void Model::translateZBy(float amount) {
    translation.z += amount;
}

void Model::rotateXBy(float amount) {
    rotation.x += glm::radians(amount);
}

void Model::rotateYBy(float amount) {
    rotation.y += glm::radians(amount);
}

void Model::rotateZBy(float amount) {
    rotation.z += glm::radians(amount);
}

GLuint Model::getVAO() {
    return vao;
}

GLuint Model::getVertexVBO() {
    return vertexVbo;
}

GLuint Model::getEBO() {
    return ebo;
}

int Model::getVertexCount() {
    return vertices.size();
}

int Model::getIndexCount() {
    return indices.size();
}

void Model::draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Model::setMaterial(Material material) {
    this->material = material;
}

void Model::toggleRed(bool on) {
    colorMask.x = on ? 1.0f : 0.0f;
}

void Model::toggleGreen(bool on) {
    colorMask.y = on ? 1.0f : 0.0f;
}

void Model::toggleBlue(bool on) {
    colorMask.z = on ? 1.0f : 0.0f;
}

void Model::setColorMask(glm::vec3 colorMask) {
    this->colorMask = colorMask;
}

void Model::setPosition(glm::vec3 position) {
    this->translation = position;
}

void Model::setScale(glm::vec3 scale) {
    this->scale = scale;
}
