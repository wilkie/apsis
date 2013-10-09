#ifdef GL_ES
precision highp float;
#endif

varying vec3 Normal;
varying vec2 Texcoord;

uniform vec4 color;
uniform sampler2D tex;

void main() {
  gl_FragColor = color * texture2D(tex, Texcoord);
}
