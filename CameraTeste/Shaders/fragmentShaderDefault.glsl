#version 400 core 

uniform sampler2D arrayOfTextures[10];
uniform float arrayOfBlendingValues[10];
uniform int numberOfTextures;

uniform vec3 lightsPos[10];
uniform vec3 lightsColorValues[10];
uniform float lightIntensity[10];
uniform mat3 lightModelMat[10];
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
    
    vec3 totalDiffuseLight = vec3(0.0);
    vec3 totalSpecularLight = vec3(0.0);

    vec3 totalAmbientLight = vec3(0.0);
    float ambientLightC = 0.005;

    float specularShininess = 256.0;
    float specularIntensity = 1.0;


    vec3 eyePosition = v;
    vec3 fragmentPosition = myVertex.xyz ;
    vec3 viewDirection = normalize(eyePosition - fragmentPosition);

    int indexOfClosestLight = 0;
    float minDistance = -1;
    for (int i = 0; i < numberOfLights; i++)
    {
        vec3 lightToFragmentVector = (lightModelMat[i] * lightsPos[i]) - (fragmentPosition);
        vec3 lightDirection = normalize(lightToFragmentVector);

        float distanceToLight = length(lightToFragmentVector)/2;
        float attenuation = 1.0 / (1.0 + 0.1*distanceToLight + 0.01*distanceToLight*distanceToLight);

        float diffuseFactor = max(dot(transformedNormal, lightDirection), 0.0);
        totalDiffuseLight += diffuseFactor * lightIntensity[i] * attenuation * lightsColorValues[i];

        vec3 reflectedLight = normalize(reflect(-lightDirection, transformedNormal));
        totalSpecularLight += specularIntensity * attenuation * pow(max(dot(viewDirection, reflectedLight), 0.0), specularShininess) * lightsColorValues[i];

        totalAmbientLight +=  1/numberOfLights * lightsColorValues[i] * min(diffuseFactor,ambientLightC)  * lightIntensity[i]  *  (attenuation > 0 ? attenuation : max(attenuation, ambientLightC));

        if(minDistance == -1 || distanceToLight < minDistance)
            indexOfClosestLight = i;
    }
  //  if(totalAmbientLight.x+ totalAmbientLight.y + totalAmbientLight.z < ambientLightC * 3)
    //{
      //  totalAmbientLight = (ambientLightC) * lightsColorValues[indexOfClosestLight];
    //}
    vec4 baseColor = (color.a > 0.0) ? mix(blendedTextureColor, color, 0.1) : color;
    FragColor = baseColor * vec4(totalDiffuseLight, 1.0) + vec4(totalSpecularLight, 1.0) + ambientLightC;
}
