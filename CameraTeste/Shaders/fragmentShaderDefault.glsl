#version 400 core

uniform sampler2D arrayOfTextures[10]; 
uniform float arrayOfBledingValues[10];
uniform int numberOfTextures;         

out vec4 FragColor;
in vec2 TextureCoord;

void main()
{
    // Initialize the final color with the first texture
    vec4 finalColor = texture(arrayOfTextures[0], TextureCoord);

    for(int i = 1; i < numberOfTextures; i++)
    {
        finalColor = mix(finalColor, texture(arrayOfTextures[i], TextureCoord), arrayOfBledingValues[i]);
    }

    FragColor = finalColor; 
}
