#version 100

in vec3 Normal;
in vec2 Texcoord;

uniform sampler2D tex;

void main() {
  gl_FragColor = texture2D(tex, Texcoord);
}
