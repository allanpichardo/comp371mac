
#version 330 core
  
layout (location = 0) in vec3 position;

uniform mat4 u_projection;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0) * u_projection;
}
