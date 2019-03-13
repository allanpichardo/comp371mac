
#version 330 core

struct Light {
    bool is_enabled;
    vec3 color;
    vec3 position;
    mat4 transformation;
};

struct Material {
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

layout (location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

uniform Light sm_light;
uniform bool is_shadow_map;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 view_position;

out vec3 fragment_position;
out vec3 fragment_normal;

void main()
{
    fragment_normal = mat3(model) * normals;
    fragment_position =  mat3(model) * position;

    if(is_shadow_map) {
        gl_Position = sm_light.transformation * model * vec4(position, 1.0f);
    } else {
        gl_Position = projection * view * model * vec4(position, 1.0f);
    }
}
