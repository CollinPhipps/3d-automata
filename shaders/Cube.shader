#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

out vec3 color;

void main()
{
	color = vec3(1.0, 1.0, 1.0);
}