#version 100

in vec3 Normal;
in vec2 Texcoord;

uniform vec4 color;
uniform sampler2D tex;

void main() {
  gl_FragColor = color * texture2D(tex, Texcoord);
}
