
#version 330 core

struct Light {
    int shadingMode;
    bool is_enabled;
    vec3 color;
    vec3 position;
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

uniform Light light;
uniform Material material;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 view_position;

out vec3 fragment_position;
out vec3 fragment_normal;
out vec3 gouraud; //computed color via goraud method

void main()
{
    fragment_normal = mat3(model) * normals;
    fragment_position =  mat3(model) * position;

    if(light.shadingMode == 0) {
        //Ambient
        vec3 ambient = material.ambient * light.color;

        //Diffuse
        vec3 light_direction = normalize(light.position - fragment_position);
        float diffuse_strength = max(dot(normalize(fragment_normal), light_direction), 0.0f);
        vec3 diffuse = (material.diffuse * diffuse_strength) * light.color;

        //Specular
        vec3 view_direction = normalize(view_position - fragment_position);
        vec3 reflect_light_direction = reflect(-light_direction, normalize(fragment_normal));
        float specular_strength = pow(max(dot(reflect_light_direction, view_direction), 0.0f), material.shininess);
        vec3 specular = (material.specular * specular_strength) * light.color;

        vec3 sda = light.is_enabled ? (specular + diffuse + ambient) : ambient;

        gouraud = sda * material.color;
    }

    gl_Position = projection * view * model * vec4(position, 1.0f);
}
