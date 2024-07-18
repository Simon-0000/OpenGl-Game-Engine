#version 330 core 

const int DIRECTIONAL_LIGHT = 0;
const int POSITIONAL_LIGHT = 1;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct Light {
    vec3 positionOrDirection;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int lightType;
};


//INPUT
in vec3 Normal;
in vec3 FragmentPosition;  
in vec2 Uv;


//OUTPUT
out vec4 FragColor; 


//UNIFORMS
uniform vec3 uViewPosition;
uniform Material uMaterial;
uniform Light uLight;



void main()
{
    vec3 diffuseVec =  vec3(texture(uMaterial.diffuse,Uv));
    //ambient 
    vec3 ambient = diffuseVec * uLight.ambient;


    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir;
    if (uLight.lightType == POSITIONAL_LIGHT){
        lightDir = normalize(uLight.positionOrDirection - FragmentPosition);  
    }else if (uLight.lightType == DIRECTIONAL_LIGHT){
        lightDir = normalize(-uLight.positionOrDirection);
    }

    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = uLight.diffuse * diff * diffuseVec;

    //specular
    vec3 viewDir = normalize(uViewPosition - FragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir,reflectDir), 0.0),uMaterial.shininess);//the 32 makes it more targeted towards a small spot 
    vec3 specular = vec3(texture(uMaterial.specular,Uv)) * spec * uLight.specular;  

    FragColor = vec4(ambient + diffuse + specular, 1.0);

}