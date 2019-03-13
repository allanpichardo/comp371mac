//
// Created by Allan Pichardo on 2019-02-27.
//

#include "light.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <light.h>
#include <vector>
#include "model.h"


Light::Light(GLuint shader, std::string uniformName, glm::vec3 position, glm::vec3 color) {
    this->shader = shader;
    this->position = position;
    this->color = color;
    this->uniformName = uniformName;

    glm::mat4 projection = glm::perspective(90.0f, (float)SHADOW_WIDTH/SHADOW_HEIGHT, 1.0f, 100.0f);
    glm::mat4 view = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    this->transformationMatrix = projection * view;

    createDepthMap();
    updateUniform();
}

void Light::updateUniform() {
    glUniform3fv(glGetUniformLocation(shader, (uniformName+".color").c_str()), 1, glm::value_ptr(color));
    glUniform3fv(glGetUniformLocation(shader, (uniformName+".position").c_str()), 1, glm::value_ptr(position));
    glUniform1i(glGetUniformLocation(shader, (uniformName+".is_enabled").c_str()), enabled);
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
    glUniform1i(glGetUniformLocation(shader, (uniformName+".shading_mode").c_str()), static_cast<int>(shadingMode));
}

void Light::createDepthMap() {
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::renderToDepthMap(std::vector<Model> &models) {
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    for(int i = 0; i < models.size(); i++) {
        models[i].draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
