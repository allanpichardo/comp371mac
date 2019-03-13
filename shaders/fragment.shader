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

out vec4 result;

//The shadow-mapped light
uniform Light sm_light;

//Not shadow-mapped lights
uniform Light light1;
uniform Light light2;
uniform Light light3;
uniform Light light4;

uniform Material material;
uniform vec3 view_position;

in vec3 fragment_position;
in vec3 fragment_normal;
in vec3 gouraud; //color computed in vertex shader

//functions
vec3 calculatePhrongModel(in Light light, in Material material) {
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

    vec3 phrong = light.is_enabled ? (specular + diffuse + ambient) : ambient;

    vec3 color = phrong * material.color;
    return color;
}

void main()
{
    vec3 color = calculatePhrongModel(light1, material) + calculatePhrongModel(light2, material) + calculatePhrongModel(light3, material) + calculatePhrongModel(light4, material);
    result = vec4(color, 1.0f);
}
