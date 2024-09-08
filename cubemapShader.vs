#version 330 core  

//INPUT
layout (location = 0) in vec3 position;  

//OUTPUT
out vec3 Uv;

//UNIFORMS
uniform mat4 uViewSkybox;
uniform mat4 uProjection;


void main()  
{  
	Uv = position;
	gl_Position = uProjection * uViewSkybox * vec4(position, 1.0);  
} 