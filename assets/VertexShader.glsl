#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec3 atexCoord;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

uniform float t;

out vec3 texCoord;

void main()
{
	texCoord = atexCoord;
	gl_Position = p * v * m * (vec4(vertexPosition, 1.0) - 0.01* vec4(0,0,t,0));
}
