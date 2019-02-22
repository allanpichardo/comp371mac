/**
 Allan Pichardo
 #40051123
 COMP 371
 Assignment 1
 
 This abstraction manipulates the view matrix through
 translation, and rotation
 */

#include "headers/camera.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(int uniformLocation, glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
    this->uniformLocation = uniformLocation;
    this->position = position;
    this->direction = direction;
    this->up = up;
    this->currentPitch = 0.0f;
    this->currentYaw = 0.0f;
    
    updateUniform();
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
    move(-1.0f * glm::cross(up, direction));
}

void Camera::panRight(){
    move(glm::cross(up, direction));
}

void Camera::panForward(){
    move(direction);
}

void Camera::panBackward(){
    move(-1.0f * direction);
}

void Camera::rotateLeft(){
    rotate(currentPitch, 1.0f);
}

void Camera::rotateRight(){
    rotate(currentPitch, -1.0f);
}

void Camera::rotateUp(){
    rotate(1.0f, currentYaw);
}

void Camera::rotateDown(){
    rotate(-1.0f, currentYaw);
}
