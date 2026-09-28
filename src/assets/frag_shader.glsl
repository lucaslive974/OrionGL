#version 330 core

struct LightScaling {
  float ambient;
  float diffuse;
  float specular;
};

struct LightAttenuation {
  float constant;
  float linear;
  float quadratic;
};

struct Light {
  vec3 position;
  vec3 direction;
  vec3 color;
  float cutOff;
  LightAttenuation attenuation;
};

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
  float shininess;
};

in vec3 FragPos;
in vec3 NormalPos;
in vec2 TextCoord;

uniform LightScaling lightScaling;

uniform Light directional;
uniform bool hasDirectional;

uniform Light[8] pointLights;
uniform int nPointLights;

uniform Material material;
uniform vec3 viewPos;

out vec4 FragColor;

vec3 calcAmbientColor(vec3 diff_textel);
vec3 calcPointLight(Light light, vec3 normal, vec3 view, vec3 diff_textel, vec3 spec_textel);
vec3 calcDirectionalLight(vec3 normal, vec3 view, vec3 diff_textel, vec3 spec_textel);
float calcAttenuation(Light);

void main()
{
  vec4 textel1 = texture(material.diffuse, TextCoord);
  vec4 textel2 = texture(material.specular, TextCoord);

  if(textel1.a < 0.1)
    discard;

  vec3 normalVector = normalize(NormalPos);
  vec3 viewVector = normalize(viewPos - FragPos);

  vec3 pixel = calcAmbientColor(vec3(textel1));

  if(hasDirectional)
    pixel += calcDirectionalLight(normalVector, viewVector, vec3(textel1), vec3(textel2));

  for(int i = 0; i < nPointLights; i++) 
    pixel += calcPointLight(pointLights[i], normalVector, viewVector, vec3(textel1), vec3(textel2));

  FragColor = vec4(pixel, textel1.a);
}

float calcAttenuation(Light light) {
  float distance = length(light.position - FragPos);
  LightAttenuation att_constants = light.attenuation;
  return 1.0 / (att_constants.constant + att_constants.linear * distance + att_constants.quadratic * pow(distance, 2));
}

vec3 calcPointLight(Light light, vec3 normal, vec3 view, vec3 diff_textel, vec3 spec_textel) {
  vec3 lightVector = normalize(light.position - FragPos);
  vec3 reflectVector = reflect(-lightVector, normal); 

  float diff = max(dot(normal, lightVector), 0.0);
  float spec = pow(max(dot(view, reflectVector), 0.0), 32);
  float attenuation = calcAttenuation(light);

  vec3 diffuse = lightScaling.diffuse * diff * diff_textel;
  vec3 specular = lightScaling.specular * spec * spec_textel; 

  diffuse *= attenuation;
  specular *= attenuation;

  return (diffuse + specular) * light.color;
}

vec3 calcDirectionalLight(vec3 normal, vec3 view, vec3 diff_textel, vec3 spec_textel) {
  vec3 lightVector = normalize(-directional.direction);
  vec3 reflectVector = reflect(-lightVector, normal); 

  float diff = max(dot(normal, lightVector), 0.0);
  float spec = pow(max(dot(view, reflectVector), 0.0), 32);

  vec3 diffuse = lightScaling.diffuse * diff * diff_textel; 
  vec3 specular = lightScaling.specular * spec * spec_textel; 

  return (diffuse + specular) * directional.color;
}

vec3 calcAmbientColor(vec3 diff_textel) {
  return lightScaling.ambient * diff_textel;
}
