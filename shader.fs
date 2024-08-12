#version 330 core 
const int MAX_DIRECTIONAL_LIGHTS = 5;
const int MAX_POINT_LIGHTS = 50;
const int MAX_SPOT_LIGHTS = 50;

struct Material {
	sampler2D diffuse;//0
	sampler2D specular;//1
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

struct SpotLight {
    vec3 position;
    LightColors light;

    vec3 direction;
    float angle;
    float outerAngle;

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
uniform SpotLight uSpotLights[MAX_SPOT_LIGHTS];
uniform int uDirectionalLightsCount;
uniform int uPointLightsCount;
uniform int uSpotLightsCount;



vec4 calculateAmbientDiffuseSpecular(vec3 lightDir,  LightColors light);
vec4 calculateDirectional(DirectionalLight directionalLight);
vec4 calculatePoint(PointLight pointLight);
vec4 calculateSpotlight(SpotLight spot);


void main()
{
    vec4 finalColor = vec4(0);
    for(int i = 0; i < uDirectionalLightsCount; ++i)
        finalColor += calculateDirectional(uDirectionalLights[i]);
    for(int i = 0; i < uPointLightsCount; ++i)
        finalColor += calculatePoint(uPointLights[i]);
    for(int i = 0; i < uSpotLightsCount; ++i)
        finalColor += calculateSpotlight(uSpotLights[i]);

//    if(finalColor.a < 0.05)
//        discard;
    FragColor = finalColor;
}


vec4 calculateAmbientDiffuseSpecular(vec3 lightDir,  LightColors light)
{
    vec4 diffuseVec =  texture(uMaterial.diffuse,Uv);
    
    //ambient 
    vec4 ambient = diffuseVec * vec4(light.ambient,1.0);
    //diffuse
    vec3 norm = normalize(Normal);
    
    float diff = max(dot(norm,lightDir), 0.0);
    vec4 diffuse = vec4(light.diffuse * diff,1.0) * diffuseVec;
    
    //specular
    vec3 viewDir = normalize(uViewPosition - FragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir,reflectDir), 0.0),uMaterial.shininess);
    vec4 specular = texture(uMaterial.specular,Uv) * vec4(spec * light.specular,0.0);  
    
    return ambient + diffuse + specular;
}

vec4 calculateDirectional(DirectionalLight directionalLight)
{
    return calculateAmbientDiffuseSpecular(normalize(-directionalLight.direction),directionalLight.light);
}

vec4 calculatePoint(PointLight pointLight)
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

vec4 calculateSpotlight(SpotLight spot){   
    vec3 lightDir = spot.position - FragmentPosition;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    float angle = dot(lightDir, normalize(-spot.direction));
    vec4 spotLightColor = vec4(0);

    

    if(angle > spot.outerAngle) {
        float attenuation = 1.0/(
                        spot.constant +
                        distance * spot.linear + 
                        (distance*distance) * spot.quadratic
                   );
        float epsilon = spot.angle - spot.outerAngle;
        float intensity = clamp((angle - spot.outerAngle) / epsilon, 0.0, 1.0);    
        spotLightColor = attenuation * intensity * calculateAmbientDiffuseSpecular(lightDir,spot.light);
    }
    return spotLightColor;
}