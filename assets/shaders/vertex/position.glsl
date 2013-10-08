#version 100

attribute vec3 normal;
attribute vec3 position;
attribute vec2 texcoord;

varying vec2 Texcoord;
varying vec3 Normal;
varying vec3 Position;

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