#version 150

in vec3 Normal;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;

vec4 directional_lighting(vec4);

void main() {
  outColor = directional_lighting(texture(tex, Texcoord));
}