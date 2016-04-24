#version 410 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 colour;
layout (location = 2) in vec2 uv;

out vec3 vertexColour;
out vec2 texCoord;

void main()
{
  gl_Position = vec4(position,0.0, 1.0);
  vertexColour = colour;
  texCoord = uv;
}
