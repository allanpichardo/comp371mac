#version 330 core

out vec4 result;


void main()
{
    gl_FragDepth = gl_FragCoord.z;
    gl_FragDepth += gl_FrontFacing ? 0.0005 : 0.0;
}
