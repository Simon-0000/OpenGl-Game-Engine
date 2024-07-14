#version 330 core 

//INPUT
in vec3 Normal;
in vec3 FragmentPosition;  
in vec2 Uv;


//OUTPUT
out vec4 FragColor; 


//UNIFORMS
uniform vec3 uLightColor;
uniform vec3 uLightPosition;
uniform vec3 uViewPosition;

uniform sampler2D texture0;


void main()
{

    //ambient 
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;


    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPosition - FragmentPosition);  
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPosition - FragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir,reflectDir), 0.0),32);//the 32 makes it more targeted towards a small spot 
    vec3 specular = specularStrength * spec * uLightColor;  

    FragColor = texture(texture0,Uv) * vec4(ambient + diffuse + specular, 1.0);

}