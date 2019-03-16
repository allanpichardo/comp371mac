
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

struct Material {
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

layout (location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texture_coords;

uniform Light sm_light;
uniform bool draw_to_shadowmap;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 view_position;

out vec3 fragment_position;
out vec3 fragment_normal;
out vec4 fragment_lightspace_position;
out vec2 fragment_texture_coords;

void main()
{
    fragment_normal = mat3(model) * normals;
    fragment_position =  mat3(model) * position;
    fragment_texture_coords = texture_coords;
    fragment_lightspace_position = sm_light.transformation * model * vec4(position, 1.0f);

    gl_Position = projection * view * model * vec4(position, 1.0f);
}
