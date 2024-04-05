#version 330 core

uniform sampler2D txt;

#define E 2.71828

in vec3 texCoord;
uniform float t;

uniform float brightness;

out vec4 color;

void main()
{
  //vec4 fog_color = vec4(0.,0.6,0.86,0.);
  //float lmb = pow(E, -0.0005 * t);
  color = texture(txt, vec2(texCoord.x, texCoord.y));
  //color = lmb*color + (1 - lmb)*fog_color;
  //color.a = 0.0;
}
