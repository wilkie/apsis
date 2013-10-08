#version 150

in vec2 Texcoord;

uniform sampler2D tex;

vec4 directional_lighting(vec4);

void main() {
  gl_FragColor = directional_lighting(texture(tex, Texcoord));
}
