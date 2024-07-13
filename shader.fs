#version 330 core 

//INPUT
in vec2 Uv;
in vec3 Normal;
in vec3 FragmentPosition;  



//OUTPUT
out vec4 FragColor; 


//UNIFORMS
uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform vec3 uLightPosition;



void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPosition - FragmentPosition);  
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;
    vec3 result = (ambient + diffuse) * uObjectColor;
    FragColor = vec4(result, 1.0);




}