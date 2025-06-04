#version 330 core

out vec4 outFragColor;

uniform int atlasIndex;

uniform sampler2DArray atlas;

in vec2 UV;

void main() {
   outFragColor = vec4(texture(atlas, vec3(UV, atlasIndex)).xyzw);
}
