#version 330 core

layout( location = 0 ) in vec4 a_pos;
layout( location = 1 ) in vec4 a_color;

uniform mat4 u_MVP;

flat out vec4 vertex_color;

void main()
{
	gl_Position = u_MVP * a_pos;
	vertex_color = a_color;
}