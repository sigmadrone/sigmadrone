#version 330

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec2 vUV;

out vec4 exColor;
out vec3 exNormal;
out vec2 UV;
uniform float scale = 1.0;
float gl_ClipDistance[1]; // declare use of 1 clip plane.
uniform vec4 Plane = vec4(0, 1, 0, -0.5); // A, B, C, and D for Ax + By + Cz + D = 0
uniform mat4 M = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0);

uniform mat3 NormalMatrix = mat3(1.0,0.0,0.0,
                                 0.0,1.0,0.0,
                                 0.0,0.0,1.0); // to transform normals, pre-perspective

void main(void)
{
	exNormal = normalize(NormalMatrix * vNormal);
	gl_Position = M*vPosition;
	exColor = vColor;
	UV = vUV;
}


