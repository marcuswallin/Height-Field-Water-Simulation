#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 exNormal;
out vec3 exSurface;
uniform sampler2D waterHeight;

uniform int grid_x;
uniform int grid_z;
uniform int resolution;



uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

void main(void)
{

	float tex_x = mod(float(inPosition.x)+0.001 , grid_x / resolution ) / (grid_x / resolution);
	
	float tex_z = mod(float(inPosition.z)+0.001, grid_z / resolution) / (grid_z / resolution);
	mat4 mdlMat = mdlMatrix;
	vec4 heights = texture(waterHeight, vec2(tex_x, tex_z));
	
	float water_depth = heights.x;
	if (water_depth < 0.001){
		water_depth = -0.1;
		}

    mdlMat[3][1] = heights.y + water_depth;
	
	exSurface = (mdlMat * vec4(inPosition, 1.0)).xyz;
	gl_Position = projMatrix * camMatrix *mdlMat * vec4(inPosition, 1.0);


	mat3 normalMatrix = mat3(mdlMat);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);
	exNormal = normalMatrix * inNormal;
	texCoord = inTexCoord;

}
