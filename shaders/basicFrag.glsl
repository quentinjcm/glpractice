#version 410

uniform float time;
in vec3 vertexColour;
in vec2 texCoord;

out vec4 outCol;

uniform sampler2D tex;

void main()
{
  outCol = texture(tex, texCoord.st);

  //outCol = vec4(texCoord, 0.0, 1.0);
  //vec4(vertexColour, 1.0);
  //outCol *= sin(time * 10);
}
