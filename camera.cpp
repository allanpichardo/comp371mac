/**
 Allan Pichardo
 #40051123
 COMP 371
 Assignment 2
 
 This abstraction manipulates the view matrix through
 translation, and rotation
 */

#include "headers/camera.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(int shader, float aspectRatio, glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
    int viewLocation = glGetUniformLocation(shader, "view");

    this->uniformLocation = viewLocation;
    this->position = position;
    this->direction = direction;
    this->up = up;
    this->currentPitch = 0.0f;
    this->currentYaw = 0.0f;

    int projectionLocation = glGetUniformLocation(shader, "projection");
    glm::mat4 projection = glm::perspective(90.0f, aspectRatio, 0.1f, 500.0f);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
    
    updateUniform();
}

glm::vec3 Camera::getPosition() {
    return position;
}

void Camera::updateUniform() {
    cameraMatrix = glm::lookAt(position, position + direction, up);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &cameraMatrix[0][0]);
}

void Camera::move(glm::vec3 vector) {
    position += vector;
    updateUniform();
}

void Camera::rotate(float pitch, float yaw) {
    direction.x += yaw;
    direction.y += pitch;
    
    updateUniform();
}

void Camera::panLeft(){
    move(-movementAmount * glm::cross(up, direction));
}

void Camera::panRight(){
    move(glm::cross(up, direction));
}

void Camera::panForward(){
    move(direction);
}

void Camera::panBackward(){
    move(-movementAmount * direction);
}

void Camera::rotateLeft(){
    rotate(currentPitch, movementAmount);
}

void Camera::rotateRight(){
    rotate(currentPitch, -movementAmount);
}

void Camera::rotateUp(){
    rotate(movementAmount, currentYaw);
}

void Camera::rotateDown(){
    rotate(-movementAmount, currentYaw);
}
