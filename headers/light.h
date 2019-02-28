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

enum ShadingMode {
    gouraud = 0,
    phrong = 1,
    normal = 2
};

class Light {
private:
    bool enabled = true;
    ShadingMode shadingMode;
    GLuint shader;
    glm::vec3 position;
    glm::vec3 color;

    void updateUniform();

public:
    Light(GLuint shader, glm::vec3 position, glm::vec3 color);
    void setEnabled(bool);
    bool isEnabled() {return enabled;};
    ShadingMode getShadingMode();
    void setShadingMode(ShadingMode shadingMode);
};

#endif //GRAPHICS_LIGHT_H
