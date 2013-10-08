#version 100

attribute vec3 Normal;
attribute vec2 Texcoord;

uniform vec4 color;
uniform sampler2D tex;

void main() {
  gl_FragColor = color * texture2D(tex, Texcoord);
}
