#version 150

in vec3 Normal;
in vec2 Texcoord;

out vec4 outColor;

uniform vec4 color;
uniform sampler2D tex;

void main() {
  outColor = color * texture2D(tex, Texcoord);
}