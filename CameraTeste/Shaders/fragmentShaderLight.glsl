#version 400 core 


in vec4 myVertex;

uniform vec3 LightColor;

out vec4 FragColor;
in vec2 TextureCoord;
in vec3 NormalVectors;

void main()
{
   

    
    vec3 totalDiffuse = vec3(0.0);

    vec4 baseColor = vec4(LightColor, 1);
    FragColor = baseColor;
}
