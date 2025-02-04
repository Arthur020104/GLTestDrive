#version 400 core

// Struct representing a light source
struct Light 
{
    vec3 positionWorld;
    vec3 colorValue;
    float intensity;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light lights[10]; // Array of up to 10 lights
uniform int numberOfLights; // Number of active lights

uniform mat3 model3; // Model matrix in 3x3 form (for normals)
uniform mat3 view3;  // View matrix in 3x3 form (for camera space)

// Struct representing the material properties
struct Material 
{
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

uniform Material material; // Material properties uniform

// Input variables
in vec4 myVertex; // Position of the current fragment in world space
in vec2 TextureCoord; // Texture coordinates for the fragment
vec2 aTextureCoord;
in vec3 NormalVectors; // Normal vectors for the fragment

// Computed view direction vector (camera direction)
vec3 v = normalize(-vec3(view3[0][2], view3[1][2], view3[2][2])); // Normalize the camera direction
uniform vec3 cameraPos;
// Output color of the fragment
out vec4 FragColor;

// Precomputed variables
vec3 totalDiffuse = vec3(0.0); // Accumulation of specular and diffuse lighting
vec3 totalSpecular = vec3(0.0);
vec3 totalAmbientLight;

void CalculateLight() 
{
    vec3 normal = normalize(model3 * NormalVectors);
    vec3 fragPos = myVertex.xyz;
    vec3 viewDir = normalize(cameraPos - fragPos);

    for (int i = 0; i < numberOfLights; i++) 
    {
        vec3 lightVec = lights[i].positionWorld - fragPos;
        vec3 lightDir = normalize(lightVec);

        float lightDist = length(lightVec);
        float attenuation = 1.0 / (1.0 + 0.1 * lightDist + 0.01 * lightDist * lightDist);

        float diffuseStrength = max(dot(normal, lightDir), 0.0);

        vec3 diffuseColor = vec3(texture(material.diffuse, aTextureCoord)) * material.diffuseMulti * diffuseStrength;
        if (!material.hasDiffuseMap) 
            diffuseColor = vec3(material.diffuseMulti) * diffuseStrength;

        vec3 halfwayDir = normalize(lightDir + viewDir);

        vec3 specularColor = vec3(texture(material.specular, aTextureCoord)) * pow(max(dot(normal, halfwayDir), 0.0), material.brightness) * material.specularMulti;
        if (!material.hasSpecularMap) 
            specularColor = vec3(material.specularMulti) * pow(max(dot(normal, halfwayDir), 0.0), material.brightness);

        totalDiffuse += attenuation * lights[i].intensity * diffuseColor * lights[i].diffuse;
        totalSpecular += attenuation * lights[i].intensity * lights[i].specular * specularColor;
    }

    totalAmbientLight = vec3(texture(material.diffuse, aTextureCoord)) * material.ambient;
    if (!material.hasDiffuseMap)
        totalAmbientLight = vec3(material.color) * material.ambient;
}

float xyzAvg(vec3 vec)
{
    return (vec.x+vec.y+vec.z)/3.0;

}

void main() 
{
    aTextureCoord = material.repeatTexture 
        ? TextureCoord * vec2(material.repeatTextureFactor.x, material.repeatTextureFactor.z) 
        : TextureCoord;

    CalculateLight();

    vec4 emissionT = texture(material.emission, aTextureCoord);
    vec4 baseColor = material.hasDiffuseMap 
        ? texture(material.diffuse, aTextureCoord) 
        : material.color;

    if (length(totalDiffuse) + length(totalSpecular) >= length(totalAmbientLight)) 
    {
        FragColor = baseColor * (xyzAvg(totalDiffuse) < 0.9 
            ? vec4(totalDiffuse, 1.0) + vec4(totalSpecular, 1.0) 
            : normalize(vec4(totalDiffuse, 1.0)) * 0.9 * 3) + vec4(totalSpecular, 1.0);

        if (material.hasEmissionMap && emissionT.w > 0.1 && xyzAvg(vec3(emissionT)) > 0.1)
            FragColor = material.emissionMulti * emissionT;
    } 
    else 
    {
        FragColor = baseColor * vec4(totalAmbientLight, 1.0);

        if (material.hasEmissionMap)
            FragColor += material.emissionMulti * emissionT;
    }
}
