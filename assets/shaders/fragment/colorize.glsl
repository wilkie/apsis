#ifdef GL_ES
precision highp float;
#endif

varying vec2 Texcoord;

uniform sampler2D tex;

vec4 directional_lighting(vec4);

void main() {
  gl_FragColor = directional_lighting(texture(tex, Texcoord));
}
