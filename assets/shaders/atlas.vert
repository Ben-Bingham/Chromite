#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUv;

uniform mat4 mvp;

out vec2 UV;

void main() {
   gl_Position = mvp * vec4(inPos.x, inPos.y, inPos.z, 1.0);

   UV = inUv;
}
