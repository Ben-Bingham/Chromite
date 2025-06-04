#version 330 core

out vec4 outFragColor;

uniform vec4 color;

uniform sampler2DArray atlas;

in vec2 UV;

void main() {
   outFragColor = vec4(texture(atlas, vec3(UV, 1)).xyz, 1.0);
}
