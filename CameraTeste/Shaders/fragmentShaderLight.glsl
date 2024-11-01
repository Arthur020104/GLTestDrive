#version 400 core 

uniform sampler2D arrayOfTextures[10];
uniform float arrayOfBlendingValues[10];
uniform int numberOfTextures;

in vec4 myVertex;

uniform vec3 LightColor;

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

    
    vec3 totalDiffuse = vec3(0.0);

    vec4 baseColor = (blendedTextureColor.x > 0.0) ? mix(blendedTextureColor, vec4(LightColor, 1), 0.01) : vec4(LightColor, 1);
    FragColor = baseColor;
}
