#version 330 core 
const int MAX_DIRECTIONAL_LIGHTS = 5;
const int MAX_POINT_LIGHTS = 50;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct LightColors{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    vec3 direction;
    LightColors light;
};
struct PointLight {
    vec3 position;
    LightColors light;

    float constant;
    float linear;
    float quadratic;
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
uniform DirectionalLight uDirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight uPointLights[MAX_POINT_LIGHTS];
uniform int uDirectionalLightsCount;
uniform int uPointLightsCount;



vec3 calculateAmbientDiffuseSpecular(vec3 lightDir,  LightColors light);
vec3 calculateDirectional(DirectionalLight directionalLight);
vec3 calculatePoint(PointLight pointLight);

void main()
{
    vec3 finalColor = vec3(0);//calculateDirectional(uDirectionalLights[0]);
    for(int i = 0; i < uDirectionalLightsCount; ++i)
        finalColor += calculateDirectional(uDirectionalLights[i]);
    for(int i = 0; i < uPointLightsCount; ++i)
        finalColor += calculatePoint(uPointLights[i]);
    FragColor = vec4(finalColor,1.0);
}


vec3 calculateAmbientDiffuseSpecular(vec3 lightDir,  LightColors light)
{
    vec3 diffuseVec =  vec3(texture(uMaterial.diffuse,Uv));
    
    //ambient 
    vec3 ambient = diffuseVec * light.ambient;
    
    //diffuse
    vec3 norm = normalize(Normal);
    
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseVec;
    
    //specular
    vec3 viewDir = normalize(uViewPosition - FragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir,reflectDir), 0.0),uMaterial.shininess);//the 32 makes it more targeted towards a small spot 
    vec3 specular = vec3(texture(uMaterial.specular,Uv)) * spec * light.specular;  
    
    return ambient + diffuse + specular;
}

vec3 calculateDirectional(DirectionalLight directionalLight)
{
    return calculateAmbientDiffuseSpecular(normalize(-directionalLight.direction),directionalLight.light);
}

vec3 calculatePoint(PointLight pointLight)
{
    vec3 lightDir = pointLight.position - FragmentPosition;  
    
    float distance = length(lightDir);
    lightDir = normalize(lightDir);
    
    float attenuation = 1.0/(
                            pointLight.constant +
                            distance * pointLight.linear + 
                            (distance*distance) * pointLight.quadratic
                       );
    
    return attenuation * calculateAmbientDiffuseSpecular(lightDir,pointLight.light);
}
