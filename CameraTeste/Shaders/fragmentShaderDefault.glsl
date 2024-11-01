#version 400 core 

uniform sampler2D arrayOfTextures[10];
uniform float arrayOfBlendingValues[10];
uniform int numberOfTextures;

uniform vec3 lightsPosWorld[10];
uniform vec3 lightsColorValues[10];
uniform float lightIntensity[10];
uniform int numberOfLights;

uniform mat3 model3;
uniform mat3 view3;

in vec4 myVertex;

vec3 v = normalize(-vec3(view3[0][2], view3[1][2], view3[2][2]));

uniform vec3 wVec; // Unused, consider removing if not needed
uniform vec4 color;

out vec4 FragColor;
in vec2 TextureCoord;
in vec3 NormalVectors;

void main()
{
    vec4 blendedTextureColor = texture(arrayOfTextures[0], TextureCoord);
    for (int i = 1; i < numberOfTextures; i++)
    {
        blendedTextureColor = mix(blendedTextureColor, texture(arrayOfTextures[i], TextureCoord), arrayOfBlendingValues[i]);
    }

    vec3 transformedNormal = normalize(model3 * NormalVectors);
    
    vec3 totalSpecularAndDiffuse = vec3(0.0);

    float ambientLightC = 0.3;

    float specularShininess = 100;
    float specularIntensity = 0.5;


    vec3 eyePosition = v;
    vec3 fragmentPosition = myVertex.xyz ;
    vec3 viewDirection = normalize(eyePosition - fragmentPosition);

    int indexClosest = 0;
    float minDistance = 0;
    for (int i = 0; i < numberOfLights; i++)
    {
        vec3 lightToFragmentVector = (lightsPosWorld[i]) - (fragmentPosition);
        vec3 lightDirection = normalize(lightToFragmentVector);

        float distanceToLight = length(lightToFragmentVector);
        float attenuation = 1.0 / (1.0 + 0.1*distanceToLight + 0.01*distanceToLight*distanceToLight);

        float diffuseFactor = max(dot(transformedNormal, lightDirection), 0.0);
        vec3 reflectedLight = normalize(reflect(-lightDirection, transformedNormal));

        totalSpecularAndDiffuse += attenuation * lightsColorValues[i]* lightIntensity[i] * (diffuseFactor  +   
        specularIntensity * pow(max(dot(viewDirection, reflectedLight), 0.0), specularShininess));

        if(minDistance > distanceToLight)
        {
            minDistance = distanceToLight;
            indexClosest = i;
        }
    }

    vec3 totalAmbientLight = vec3(ambientLightC) * lightsColorValues[indexClosest];

    vec4 baseColor = (color.a > 0.0) ? mix(blendedTextureColor, color, 0.1) : color;
    FragColor = baseColor * (vec4(totalSpecularAndDiffuse, 1.0) + vec4(totalAmbientLight,1));
}
