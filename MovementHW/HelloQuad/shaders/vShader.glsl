#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 vertUV;
layout (location = 2) uniform mat4 locWorldMat;
out vec3 fragLoc;
out vec2 fragUV;

void main()
{
	gl_Position = locWorldMat*vec4(position, 1);
	fragLoc = position;
	fragUV = vertUV;
}