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

in vec4 myVertex;

vec3 v = normalize(-vec3(view3[0][2], view3[1][2], view3[2][2]));

uniform vec3 wVec;
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

    vec3 transformedNormal = normalize(view3 * model3 * NormalVectors);
    
    vec3 totalDiffuseLight = vec3(0.0);
    vec3 totalSpecularLight = vec3(0.0);
    float specularShininess = 25600.0;
    float specularIntensity = 2.0;

    vec3 eyePosition = v;
    vec3 fragmentPosition = myVertex.xyz / myVertex.w;
    vec3 viewDirection = normalize(eyePosition - fragmentPosition);

    for (int i = 0; i < numberOfLights; i++)
    {
        vec3 lightDirection = normalize(view3 * lightModelMat[i] * lightsWValues[i]);
        
        float diffuseFactor = max(dot(transformedNormal, lightDirection), 0.0);
        totalDiffuseLight += diffuseFactor * lightIntensity[i] * lightsColorValues[i];

        vec3 reflectedLight = normalize(reflect(-lightDirection, transformedNormal));
        totalSpecularLight += specularIntensity * pow(max(dot(viewDirection, reflectedLight), 0.0), specularShininess) * lightsColorValues[i];
    }

    vec4 baseColor = (color.a > 0.0) ? mix(blendedTextureColor, color, 0.1) : color;
    FragColor = baseColor * vec4(totalDiffuseLight, 1.0) + vec4(totalSpecularLight, 1.0);
}
