#version 330 core  

//INPUT
layout (location = 0) in vec2 position;  
layout (location = 1) in vec2 uv;  

out vec2 Uv;

void main(){
	gl_Position = vec4(position.x,position.y,0,1.0);
	Uv = uv;
}