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

uniform int shadingType;
uniform Material material;
uniform Light light;
uniform vec3 view_position;

in vec3 fragment_position;
in vec3 fragment_normal;
in vec3 gouraud; //color computed in vertex shader

void main()
{
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

    result = vec4(color, 1.0f);

}