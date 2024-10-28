#version 400 core

uniform sampler2D arrayOfTextures[10];
uniform float arrayOfBlendingValues[10];
uniform int numberOfTextures;

uniform vec3 lightsWValues[10];
uniform vec3 lightsColorValues[10];
uniform float lightIntensity[10];
uniform mat3 lightModelMat[10];
uniform int numberOfLights;

uniform mat3 model3;
uniform mat3 view3;

vec3 v = -vec3(view3[0][2] , view3[1][2], view3[2][2]);

uniform vec3 wVec;
uniform vec4 color;

out vec4 FragColor;
in vec2 TextureCoord;
in vec3 NormalVectors;

void main()
{
    vec4 finalColor = texture(arrayOfTextures[0], TextureCoord);
    for (int i = 1; i < numberOfTextures; i++)
    {
        finalColor = mix(finalColor, texture(arrayOfTextures[i], TextureCoord), arrayOfBlendingValues[i]);
    }

    vec3 transformedNormal = normalize(view3 * model3 * NormalVectors);
    
    vec3 totalDiffuse = vec3(0.0);

    for (int i = 0; i < numberOfLights; i++)
    {
        vec3 lightDir = normalize(view3 * lightModelMat[i] * lightsWValues[i]);
        
        float currentDot = max(dot(transformedNormal, lightDir), 0.0);
        totalDiffuse += currentDot * lightIntensity[i] * lightsColorValues[i];
    }

    vec4 baseColor = (color.a > 0.0 && finalColor + color != color) ? mix(finalColor, color, 0.1) : color;
    FragColor = FragColor = vec4(totalDiffuse, 1.0) * baseColor;
}
