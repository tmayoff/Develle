#type vertex
#version 450 core

struct VertexOutput {
  vec4 Color;
  vec2 TexCoord;
  vec3 Normal;
};

struct Light { 
  float u_Intensity; 
  vec3 u_Postition; 
  vec3 u_Color;
};

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Normal;

layout(std140, binding = 0) uniform Camera { mat4 u_ViewProjection; };
layout(std140, binding = 1) uniform LightBlock { 
  float u_Intensity; 
  vec3 u_Postition; 
  vec3 u_Color;
} light;

layout(location = 0) out VertexOutput Output;
layout(location = 3) out vec3 v_FragPos;
layout(location = 4) out Light v_Light;

void main() {
  Output.Color = a_Color;
  Output.TexCoord = a_TexCoord;
  Output.Normal = a_Normal;

  v_FragPos = a_Position.xyz;

  v_Light.u_Intensity = light.u_Intensity;
  v_Light.u_Postition = light.u_Postition;
  v_Light.u_Color = light.u_Color;

  gl_Position = u_ViewProjection * a_Position;
}

#type fragment
#version 450 core

struct VertexOutput {
  vec4 Color;
  vec2 TexCoord;
  vec3 Normal;
};

struct Light { 
  float u_Intensity; 
  vec3 u_Postition; 
  vec3 u_Color;
};

layout(location = 0) out vec4 color;

layout(location = 0) in VertexOutput Input;
layout(location = 3) in vec3 v_FragPos;
layout(location = 4) in Light v_Light;

layout(binding = 0) uniform sampler2D u_Texture;

void main() { 
  vec3 ambient = v_Light.u_Intensity * v_Light.u_Color;

  vec3 normal = normalize(Input.Normal);
  vec3 lightDir = normalize(v_Light.u_Postition - v_FragPos);

  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * v_Light.u_Color;

  vec4 textureTint = Input.Color * texture(u_Texture, Input.TexCoord); 
  vec4 lighting = vec4(ambient + diffuse, 1.0);
  color = textureTint * lighting;
}
