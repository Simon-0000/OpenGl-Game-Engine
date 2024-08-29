#version 330 core

out vec4 FragColor;

in vec2 Uv;
uniform sampler2D uTexture;

void main(){
    FragColor = vec4(vec3(1.0 - texture(uTexture, Uv)), 1.0);
}