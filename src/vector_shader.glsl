
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexture;

out vec3 ourNormal;
out vec3 fragmentPosition;
out vec2 ourTexture;

uniform mat4 model,view,projection;

void main()
{
	gl_Position =projection*view*model*vec4(aPos, 1.0);
	fragmentPosition=vec3(model*vec4(aPos,1.0));
	ourNormal=mat3(transpose(inverse(model)))*aNormal;
	ourTexture=vec2(aTexture.x,aTexture.y);
}
