#ifdef GL_ES
precision highp float;
#endif

varying vec3 Normal;
varying vec3 Position;

struct DirectionalLight {
  vec3 color;
  vec3 direction;
};

struct Material {
  float ambient_intensity;
  float diffuse_intensity;
  float specular_intensity;
  float shininess;
  vec3  emission;
};

uniform DirectionalLight light;
uniform Material material;

uniform vec3 camera;

vec4 ambient_lighting() {
  return vec4(light.color, 1.0) * material.ambient_intensity;
}

vec4 diffuse_lighting() {
  float factor = dot(normalize(Normal), light.direction);

  return vec4(light.color, 1.0) * material.diffuse_intensity * max(0.0, factor);
}

vec4 specular_lighting() {
  float factor = dot(normalize(Normal), light.direction);

  if (factor > 0) {
    vec3 to_eye = normalize(camera - Position);
    vec3 light_reflect = normalize(reflect(-light.direction, Normal));
    factor = dot(to_eye, light_reflect);
    factor = pow(factor, material.shininess);
    return vec4(light.color, 1.0f) * material.specular_intensity * max(0.0, factor);
  }

  return vec4(0.0, 0.0, 0.0, 0.0);
}

vec4 directional_lighting(vec4 color) {
  return color * (vec4(material.emission, 1.0) + ambient_lighting() + diffuse_lighting() + specular_lighting());
}
