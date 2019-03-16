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
    float currentPitch = 0.0f;
    float currentYaw = 0.0f;
    float movementAmount = 0.3f;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::mat4 cameraMatrix;
    glm::mat4 projection;
    
    void move(glm::vec3 vector);
    void rotate(float pitch, float yaw);
    
public:
    Camera(float aspectRatio, glm::vec3 position, glm::vec3 direction, glm::vec3 up);
    void updateUniform(int shader);
    glm::vec3 getPosition();
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
