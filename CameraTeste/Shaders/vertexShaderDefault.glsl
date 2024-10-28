#version 400 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Normals;
layout (location = 2) in vec2 TextCoord;

out vec2 TextureCoord;
out vec3 NormalVectors;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 myVertex;
void main()
{
    gl_Position = projection * view * model * vec4(Pos, 1.0);
    TextureCoord = TextCoord;
    NormalVectors = Normals;
    myVertex = model * vec4(Pos, 1.0); 
}