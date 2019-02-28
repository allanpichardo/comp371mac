//
// Created by Allan Pichardo on 2019-02-27.
//

#include "light.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <light.h>


Light::Light(GLuint shader, glm::vec3 position, glm::vec3 color) {
    this->shader = shader;
    this->position = position;
    this->color = color;

    updateUniform();
}

void Light::updateUniform() {
    glUniform3fv(glGetUniformLocation(shader, "light.color"), 1, glm::value_ptr(color));
    glUniform3fv(glGetUniformLocation(shader, "light.position"), 1, glm::value_ptr(position));
    glUniform1i(glGetUniformLocation(shader, "light.is_enabled"), enabled);
}

void Light::setEnabled(bool isEnabled) {
    enabled = isEnabled;
    updateUniform();
}

ShadingMode Light::getShadingMode() {
    return shadingMode;
}

void Light::setShadingMode(ShadingMode shadingMode) {
    this->shadingMode = shadingMode;
    glUniform1i(glGetUniformLocation(shader, "light.shadingMode"), static_cast<int>(shadingMode));
}
