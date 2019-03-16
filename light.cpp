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


Light::Light(std::string uniformName, glm::vec3 position, glm::vec3 color) {
    this->position = position;
    this->color = color;
    this->uniformName = uniformName;

    this->cutOff = glm::cos(glm::radians(30.0f));
    this->direction = glm::normalize(glm::vec3(0.0f) - position);

    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH/SHADOW_HEIGHT, 1.f, 500.0f);
    glm::mat4 view = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    this->transformationMatrix = projection * view;
}

void Light::updateUniform(int shader) {
    glUniform3fv(glGetUniformLocation(shader, (uniformName+".color").c_str()), 1, glm::value_ptr(color));
    glUniform3fv(glGetUniformLocation(shader, (uniformName+".position").c_str()), 1, glm::value_ptr(position));
    glUniform1i(glGetUniformLocation(shader, (uniformName+".is_enabled").c_str()), enabled);
    glUniform3fv(glGetUniformLocation(shader, (uniformName+".direction").c_str()), 1, glm::value_ptr(direction));
    glUniform1f(glGetUniformLocation(shader, (uniformName+".cutOff").c_str()), cutOff);
    glUniform1i(glGetUniformLocation(shader, (uniformName+".lightMode").c_str()), static_cast<int>(lightMode));
    glUniform1i(glGetUniformLocation(shader, (uniformName+".shading_mode").c_str()), static_cast<int>(shadingMode));
    glUniformMatrix4fv(glGetUniformLocation(shader, (uniformName+".transformation").c_str()), 1, GL_FALSE, &transformationMatrix[0][0]);
}

void Light::setEnabled(bool isEnabled) {
    enabled = isEnabled;
}

ShadingMode Light::getShadingMode() {
    return shadingMode;
}

void Light::setShadingMode(ShadingMode shadingMode) {
    this->shadingMode = shadingMode;
}

bool Light::createDepthMap() {
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void Light::renderToDepthMap(int shader, std::vector<Model*> &models) {
    updateUniform(shader);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    for(int i = 0; i < models.size(); i++) {
        models[i]->updateUniform(shader);
        models[i]->draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Light::getDepthMap() {
    return depthMap;
}

void Light::makeSpotlight(glm::vec3 direction, float cutoffAngle) {
    this->direction = direction;
    this->cutOff = glm::cos(glm::radians(cutoffAngle));
    this->lightMode = LightMode::spotlight;
}

glm::vec3 Light::getPosition() {
    return position;
}
