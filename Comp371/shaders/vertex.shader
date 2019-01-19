
#version 330 core
  
layout (location = 0) in vec3 position;

uniform float u_scale;
uniform vec3 u_translation;

void main()
{
    vec3 transformedPosition = (position * u_scale) + u_translation;
    gl_Position = vec4(transformedPosition.x, transformedPosition.y, transformedPosition.z, 1.0);
}