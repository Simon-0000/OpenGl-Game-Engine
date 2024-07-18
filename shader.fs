#version 330 core 
struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    vec3 lightDir = normalize(uLight.position - FragmentPosition);  
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = uLight.diffuse * diff * diffuseVec;

    //specular
    vec3 viewDir = normalize(uViewPosition - FragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir,reflectDir), 0.0),uMaterial.shininess);//the 32 makes it more targeted towards a small spot 
    vec3 specular = (uMaterial.specular * spec) * uLight.specular;  

    FragColor = vec4(ambient + diffuse + specular, 1.0);

}