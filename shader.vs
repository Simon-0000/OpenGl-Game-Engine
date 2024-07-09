#version 330 core  
layout (location = 0) in vec3 aPos;  
layout (location = 1) in vec3 aColor;  
layout (location = 2) in vec2 aTextureCoord;  

uniform mat4 transform;

out vec2 ourTextureCoord; 

void main()  
{  
	gl_Position = transform * vec4(aPos, 1.0);  
	ourTextureCoord = aTextureCoord;
} 