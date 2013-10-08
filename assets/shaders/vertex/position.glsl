#version 150

in vec3 normal;
in vec3 position;
in vec2 texcoord;

out vec2 Texcoord;
out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 camera;

void main() {
  Texcoord = texcoord;
  Normal = (model * vec4(normal, 1.0)).xyz;
  Position = (model * vec4(position, 1.0)).xyz;

  gl_Position = proj * view * model * vec4(position, 1.0);
}