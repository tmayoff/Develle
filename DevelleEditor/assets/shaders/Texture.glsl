#type vertex
#version 450 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform Camera { mat4 u_ViewProjection; };

struct VertexOutput {
  vec4 Color;
  vec2 TexCoord;
};

layout(location = 0) out VertexOutput Output;

void main() {
  Output.Color = a_Color;
  Output.TexCoord = a_TexCoord;
  gl_Position = u_ViewProjection * a_Position;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

struct VertexOutput {
  vec4 Color;
  vec2 TexCoord;
};

layout(location = 0) in VertexOutput Input;

layout(binding = 0) uniform sampler2D u_Texture;

void main() { color = Input.Color * texture(u_Texture, Input.TexCoord); }
