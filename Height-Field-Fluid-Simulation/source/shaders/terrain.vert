#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 exNormal;
out vec3 exSurface;


uniform mat4 projMatrix;
uniform mat4 mtwMatrix;
uniform mat4 camMatrix;

void main(void)
{
	exSurface = (mtwMatrix * vec4(inPosition, 1.0)).xyz;
	gl_Position = projMatrix * camMatrix* mtwMatrix * vec4(inPosition, 1.0);
	//bad naming here
	mat3 normalMatrix = mat3(camMatrix*mtwMatrix);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);
	exNormal = normalMatrix * inNormal;
	texCoord = inTexCoord;

}
