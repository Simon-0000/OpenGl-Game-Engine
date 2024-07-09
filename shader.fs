#version 330 core 
out vec4 FragColor; 
in vec2 ourTextureCoord;


uniform sampler2D texture1;
uniform sampler2D texture2;

//uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, ourTextureCoord), texture(texture2, ourTextureCoord), 0.5);
}