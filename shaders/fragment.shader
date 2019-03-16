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

out vec4 result;

//The shadow-mapped light
uniform Light sm_light;
uniform bool is_depth_map;
uniform sampler2D shadowMap;

//Not shadow-mapped lights
uniform Light light1;
uniform Light light2;
uniform Light light3;
uniform Light light4;

uniform Material material;
uniform vec3 view_position;

in vec3 fragment_position;
in vec3 fragment_normal;
in vec4 fragment_lightspace_position;

vec3 calculatePhrongModel(in Light light, in Material material, float shadow) {
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

    vec3 phrong = light.is_enabled ? ((specular + diffuse) * (1.0f - shadow)) + ambient : ambient;

    vec3 color = phrong * material.color;
    return color;
}

float calculateShadow(in vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.005 * (1.0 - dot(fragment_normal, normalize(sm_light.position - fragment_position))), 0.0005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
    if(!is_depth_map) {
        vec3 color = calculatePhrongModel(light1, material, 0.0f) + calculatePhrongModel(light2, material, 0.0f) + calculatePhrongModel(light3, material, 0.0f) + calculatePhrongModel(light4, material, 0.0f);
        result = vec4(color, 1.0f);
    }
    else {
        float shadow = calculateShadow(fragment_lightspace_position);
        float theta = dot(normalize(sm_light.position - fragment_position), normalize(-sm_light.direction));

        if(theta > sm_light.cutOff)
        {
            vec3 color = calculatePhrongModel(sm_light, material, shadow);
            result = vec4(color, 1.0f);
        }else {
            result = vec4(material.ambient * material.diffuse * sm_light.color, 1.0);
        }

    }
}
