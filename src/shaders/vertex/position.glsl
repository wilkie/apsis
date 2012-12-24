#version 150

in vec3 normal;
in vec3 position;
in vec2 texcoord;

out vec2 Texcoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
  Texcoord = texcoord;
  Normal = normal;
  gl_Position = proj * view * model * vec4(position, 1.0);
}