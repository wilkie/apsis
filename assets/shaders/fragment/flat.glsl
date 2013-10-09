#ifdef GL_ES
precision highp float;
#endif

varying vec3 Normal;
varying vec2 Texcoord;

uniform sampler2D tex;

void main() {
  gl_FragColor = texture2D(tex, Texcoord);
}
