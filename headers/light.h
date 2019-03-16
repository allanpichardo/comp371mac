/**
 * Allan Pichardo
 * COMP 371
 * Assignment 2
 *
 * This abstraction represents a light source
 */

#ifndef GRAPHICS_LIGHT_H
#define GRAPHICS_LIGHT_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <string>
#include "model.h"

enum ShadingMode {
    gouraud = 0,
    phrong = 1,
    normal = 2
};

enum LightMode {
    point = 0,
    spotlight = 1
};

class Light {
private:
    LightMode lightMode;
    std::string uniformName;
    unsigned int depthMapFBO;
    unsigned int depthMap;
    bool enabled = true;
    ShadingMode shadingMode;
    GLuint shader;
    glm::vec3 position;
    glm::vec3 color;
    glm::mat4 transformationMatrix;
    glm::vec3 direction;
    float cutOff;

public:
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    Light(std::string uniformName, glm::vec3 position, glm::vec3 color);
    void updateUniform(int shader);
    void setEnabled(bool);
    bool isEnabled() {return enabled;};
    ShadingMode getShadingMode();
    LightMode getLightMode();
    void setLightMode(LightMode lightMode);
    void setShadingMode(ShadingMode shadingMode);
    void renderToDepthMap(int shader, std::vector<Model*> &models);
    unsigned int getDepthMap();
    glm::vec3 getPosition();
    bool createDepthMap();
    void makeSpotlight(glm::vec3 direction, float cutoffAngle);
};

#endif //GRAPHICS_LIGHT_H
