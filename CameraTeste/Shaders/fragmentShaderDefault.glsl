#version 400 core

struct Light {
    vec3 positionWorld;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    bool isDirectional;
    vec3 colorValue;
    float intensity;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float brightness;
    vec4 color;
    float diffuseMulti;
    float specularMulti;
    float emissionMulti;
    bool hasSpecularMap;
    bool hasDiffuseMap;
    bool hasEmissionMap;
    bool repeatTexture;
    vec3 repeatTextureFactor;
};

uniform Light lights[10];
uniform int numberOfLights;
uniform mat3 model3;
uniform mat3 view3;
uniform Material material;
uniform vec3 cameraPos;

in vec4 myVertex;
in vec2 TextureCoord;
in vec3 NormalVectors;
out vec4 FragColor;

vec2 aTextureCoord;
vec3 totalDiffuse = vec3(0.0);
vec3 totalSpecular = vec3(0.0);
vec3 totalAmbientLight;

// Helper functions
vec3 calculateLightDirection(Light light, vec3 fragPos, out vec3 lightVec) 
{
    if(light.isDirectional) 
    {
        lightVec = vec3(0.0);
        return normalize(light.positionWorld);
    }
    lightVec = light.positionWorld - fragPos;
    return normalize(lightVec);
}

float calculateSpotIntensity(Light light, vec3 lightDir) 
{
    if(light.isDirectional || light.cutOff == -1.0) 
        return 1.0;
    
    float theta = dot(lightDir, normalize(-light.direction));
    if(theta <= light.outerCutOff) return 0.0;
    
    float epsilon = light.cutOff - light.outerCutOff;
    return clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
}

float calculateAttenuation(Light light, float distance) 
{
    return light.isDirectional ? 1.0 : 1.0 / (1.0 + 0.1*distance + 0.01*distance*distance);
}

vec3 calculateDiffuse(Light light, vec3 normal, vec3 lightDir) 
{
    float strength = max(dot(normal, lightDir), 0.0);
    vec3 texColor = material.hasDiffuseMap 
        ? texture(material.diffuse, aTextureCoord).rgb 
        : material.color.rgb;
    return texColor * material.diffuseMulti * strength * light.diffuse;
}

vec3 calculateSpecular(Light light, vec3 normal, vec3 lightDir, vec3 viewDir) 
{
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specPower = pow(max(dot(normal, halfwayDir), 0.0), material.brightness);
    vec3 specColor = material.hasSpecularMap 
        ? texture(material.specular, aTextureCoord).rgb 
        : vec3(1.0);
    return specColor * material.specularMulti * specPower * light.specular;
}

void calculateLightContribution() 
{
    vec3 normal = normalize(model3 * NormalVectors);
    vec3 fragPos = myVertex.xyz;
    vec3 viewDir = normalize(cameraPos - fragPos);

    for(int i = 0; i < numberOfLights; i++) 
    {
        Light light = lights[i];
        
        vec3 lightVec;
        vec3 lightDir = calculateLightDirection(light, fragPos, lightVec);
        
        float spotIntensity = calculateSpotIntensity(light, lightDir);
        if(spotIntensity <= 0.0) continue;
        
        float attenuation = calculateAttenuation(light, length(lightVec));
        vec3 diffuse = calculateDiffuse(light, normal, lightDir);
        vec3 specular = calculateSpecular(light, normal, lightDir, viewDir);
        
        totalDiffuse += light.intensity * attenuation * spotIntensity * diffuse;
        totalSpecular += light.intensity * attenuation * spotIntensity * specular;
    }

    totalAmbientLight = material.hasDiffuseMap 
        ? texture(material.diffuse, aTextureCoord).rgb * material.ambient 
        : material.color.rgb * material.ambient;
}

float calculateLuminance(vec3 color) 
{
    return (color.r + color.g + color.b) / 3.0;
}

vec4 calculateEmission() 
{
    if(!material.hasEmissionMap) return vec4(0.0);
    
    vec4 emission = texture(material.emission, aTextureCoord);
    if(emission.a > 0.1 && calculateLuminance(emission.rgb) > 0.1) 
    {
        return material.emissionMulti * emission;
    }
    return vec4(0.0);
}

void main() 
{
    aTextureCoord = material.repeatTexture 
        ? TextureCoord * vec2(material.repeatTextureFactor.x, material.repeatTextureFactor.y)
        : TextureCoord;

    calculateLightContribution();
    
    vec4 baseColor = material.hasDiffuseMap 
        ? texture(material.diffuse, aTextureCoord) 
        : material.color;
    
    vec4 emission = calculateEmission();
    vec4 ambientColor = vec4(totalAmbientLight, 1.0) * baseColor;
    
    if(emission.a > 0.0) 
    {
        FragColor = emission;
        return;
    }

    if(length(totalDiffuse + totalSpecular) >= length(totalAmbientLight)) 
    {
        float lum = calculateLuminance(totalDiffuse);
        vec3 adjustedDiffuse = lum < 0.9 ? totalDiffuse : normalize(totalDiffuse) * 0.9 * 3.0;
        FragColor = baseColor * vec4(adjustedDiffuse + totalSpecular, 1.0);
    } 
    else 
    {
        FragColor = ambientColor;
        if(material.hasEmissionMap) 
        {
            FragColor += calculateEmission();
        }
    }
}