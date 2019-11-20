#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 exNormal;
out vec3 exSurface;
uniform sampler2D waterHeight;


uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

void main(void)
{
   	//hard coded for now
	int offset_x = 60;
    int offset_z = 60;

	float tex_x = mod(inPosition.x - offset_x, 20) / 20;
	float tex_z = mod(inPosition.z - offset_z, 20) / 20;
	mat4 mdlMat = mdlMatrix;
    mdlMat[3][1] = mdlMatrix[3][1]+ texture(waterHeight, vec2(tex_x, tex_z)).x / 10;

	exSurface = (mdlMat * vec4(inPosition, 1.0)).xyz;
	gl_Position = projMatrix * mdlMat * vec4(inPosition, 1.0);


	mat3 normalMatrix = mat3(mdlMat);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);
	exNormal = normalMatrix * inNormal;
	texCoord = inTexCoord;

}
