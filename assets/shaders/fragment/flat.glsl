#version 100

attribute vec3 Normal;
attribute vec2 Texcoord;

uniform sampler2D tex;

void main() {
  gl_FragColor = texture2D(tex, Texcoord);
}
