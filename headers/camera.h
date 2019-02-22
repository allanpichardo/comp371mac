/**
 Allan Pichardo
 #40051123
 COMP 371
 Assignment 1
 
 This abstraction manipulates the view matrix through
 translation, and rotation
 */

#ifndef camera_h
#define camera_h

#include <glm/glm.hpp>

class Camera {
    int uniformLocation = -1;
    float currentPitch = 0.0f;
    float currentYaw = 0.0f;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::mat4 cameraMatrix;
    
    void updateUniform();
    void move(glm::vec3 vector);
    void rotate(float pitch, float yaw);
    
public:
    Camera(int uniformLocation, glm::vec3 position, glm::vec3 direction, glm::vec3 up);
    void panLeft();
    void panRight();
    void panForward();
    void panBackward();
    void rotateLeft();
    void rotateRight();
    void rotateUp();
    void rotateDown();
};

#endif /* camera_h */
