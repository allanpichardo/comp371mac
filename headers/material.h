/**
 * Allan Pichardo
 * COMP 371
 * Assignment 2
 *
 * This structure contains information about
 * the material of an object. It can be optionally
 * added to a Model object
 */

#ifndef GRAPHICS_MATERIAL_H
#define GRAPHICS_MATERIAL_H

#include <glm/glm.hpp>

class Material {
public:
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec3 ambient = glm::vec3(1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(1.0f);
    float shininess = 32.0f;
};

#endif //GRAPHICS_MATERIAL_H
