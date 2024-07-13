#version 330 core  

//INPUT
layout (location = 0) in vec3 position;  
layout (location = 1) in vec2 uv;  
layout (location = 2) in vec3 normal;


//OUTPUT
out vec2 Uv;
out vec3 Normal;
out vec3 FragmentPosition;


//UNIFORMS
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;


void main()  
{  
	FragmentPosition = vec3(uModel * vec4(position, 1.0));
	gl_Position = uProjection * uView * uModel * vec4(position, 1.0);  
	Uv = uv;
	Normal = mat3(transpose(inverse(uModel))) * normal;//inversing is costly, should calculate it once on cpu instead
} 