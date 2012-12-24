#version 150

in vec3 Normal;

struct DirectionalLight {
  vec3 color;
  vec3 direction;
  float ambient_intensity;
  float diffuse_intensity;
};

uniform DirectionalLight light;

vec4 ambient_lighting() {
  return vec4(light.color, 1.0) * light.ambient_intensity;
}

vec4 diffuse_lighting() {
  float factor = dot(normalize(Normal), -light.direction);

  if (factor > 0) {
    return vec4(light.color, 1.0) * light.diffuse_intensity * factor;
  }
  else {
    return vec4(0.0, 0.0, 0.0, 0.0);
  }
}

vec4 directional_lighting(vec4 color) {
  return color * (ambient_lighting() + diffuse_lighting());
}