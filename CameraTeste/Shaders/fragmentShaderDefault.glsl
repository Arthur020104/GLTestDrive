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
    float roughness;
    vec4 color;
}; 

uniform Material material; // Material properties uniform

// Input variables
in vec4 myVertex; // Position of the current fragment in world space
in vec2 TextureCoord; // Texture coordinates for the fragment
in vec3 NormalVectors; // Normal vectors for the fragment

// Computed view direction vector (camera direction)
vec3 v = normalize(-vec3(view3[0][2], view3[1][2], view3[2][2])); // Normalize the camera direction
uniform vec3 cameraPos;
// Output color of the fragment
out vec4 FragColor;

// Precomputed variables
vec3 totalSpecularAndDiffuse = vec3(0.0); // Accumulation of specular and diffuse lighting
vec3 totalAmbientLight;

void CalculateLight()
{
    vec3 transformedNormal = normalize(model3 * NormalVectors); // Transform normals
    vec3 fragmentPosition = myVertex.xyz; // Fragment position in world space
    vec3 viewDirection = normalize(cameraPos - fragmentPosition); // View direction relative to camera

    int indexClosest = 0;
    float minDistance = 0;

    for (int i = 0; i < numberOfLights; i++) 
    {
        vec3 lightToFragmentVector = lights[i].positionWorld - fragmentPosition;
        vec3 lightDirection = normalize(lightToFragmentVector);

        float distanceToLight = length(lightToFragmentVector);
        float attenuation = 1.0 / (1.0 + 0.1 * distanceToLight + 0.01 * distanceToLight * distanceToLight);

        float diffuseFactor = max(dot(transformedNormal, lightDirection), 0.0);
        vec3 materialScaledDiffuse = vec3(texture(material.diffuse, TextureCoord));
        if (materialScaledDiffuse == vec3(0.0)) 
        {
            materialScaledDiffuse = vec3(1) * diffuseFactor;
        }

        vec3 halfVector = normalize(lightDirection + viewDirection); // Halfway vector
        float specularFactor = pow(max(dot(transformedNormal, halfVector), 0.0), material.roughness);

        totalSpecularAndDiffuse += attenuation * lights[i].intensity * 
                                   (materialScaledDiffuse * lights[i].diffuse +
                                   vec3(texture(material.specular, TextureCoord)) * lights[i].specular * specularFactor);

        if (minDistance > distanceToLight || i == 0) 
        { 
            minDistance = distanceToLight;
            indexClosest = i;
        }
    }

    totalAmbientLight = vec3(texture(material.diffuse, TextureCoord)) * lights[indexClosest].ambient;
}


void main() 
{
    CalculateLight();

    vec4 baseColor = (vec3(texture(material.diffuse, TextureCoord)) != vec3(0.0)) 
                     ? vec4(vec3(texture(material.diffuse, TextureCoord)) * 0.9, 1.0) 
                     : material.color;

    FragColor = baseColor * (vec4(totalSpecularAndDiffuse, 1.0) + vec4(totalAmbientLight, 1.0));
}
