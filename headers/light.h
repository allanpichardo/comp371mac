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

class Light {
private:
    std::string uniformName;
    unsigned int depthMapFBO;
    unsigned int depthMap;
    bool enabled = true;
    ShadingMode shadingMode;
    GLuint shader;
    glm::vec3 position;
    glm::vec3 color;
    glm::mat4 transformationMatrix;

    void updateUniform();
    void createDepthMap();

public:
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    Light(GLuint shader, std::string uniformName, glm::vec3 position, glm::vec3 color);
    void setEnabled(bool);
    bool isEnabled() {return enabled;};
    ShadingMode getShadingMode();
    void setShadingMode(ShadingMode shadingMode);
    void renderToDepthMap(std::vector<Model*> &models);
};

#endif //GRAPHICS_LIGHT_H
