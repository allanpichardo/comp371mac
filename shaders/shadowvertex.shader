
#version 330 core

struct Light {
    int lightMode;
    bool is_enabled;
    vec3 color;
    vec3 position;
    mat4 transformation;
    vec3 direction;
    float cutOff;
};

layout (location = 0) in vec3 position;

uniform Light sm_light;
uniform mat4 model;

void main()
{
    gl_Position = sm_light.transformation * model * vec4(position, 1.0f);
}
