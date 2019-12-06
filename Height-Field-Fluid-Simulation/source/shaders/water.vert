#version 150

in  vec3 inPosition;
//in  vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 exNormal;
out vec3 exSurface;
out float water_height;
//out vec3 exSurface;
uniform sampler2D waterHeight;

uniform int grid_x;
uniform int grid_z;
uniform int resolution;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

void main(void)
{
    exSurface = (camMatrix * mdlMatrix * vec4(inPosition, 1.0)).xyz;

    int grid_res_x = grid_x / resolution;
    int grid_res_z = grid_z / resolution;

	//can add more here later
	float tex_x = float((inPosition.x)+0.001)  / grid_res_x;
	float tex_z = float((inPosition.z)+0.001) / grid_res_z;
	float tex_x_plus = float((inPosition.x+1.0/resolution)+0.001) / grid_res_x;
	float tex_z_plus = float((inPosition.z+1.0/resolution)+0.001) / grid_res_z;
	float tex_x_minus =float((inPosition.x-1.0/resolution)+0.001) / grid_res_x;
	float tex_z_minus = float((inPosition.z-1.0/resolution)+0.001) / grid_res_z;

	vec4 heights = texture(waterHeight, vec2(tex_x, tex_z));
	water_height = heights.x;
	vec4 h_xplus = texture(waterHeight, vec2(tex_x_plus, tex_z));
	vec4 h_xminus = texture(waterHeight, vec2(tex_x_minus, tex_z));
	vec4 h_zplus = texture(waterHeight, vec2(tex_x, tex_z_plus));
	vec4 h_zminus = texture(waterHeight, vec2(tex_x, tex_z_minus));

	vec3 vec_h = vec3(0, heights.x + heights.y,0);
	
	vec3 vec_xplus = vec3(1, h_xplus.x + h_xplus.y,0) - vec_h;
	vec3 vec_xminus = vec3(-1 , h_xminus.x + h_xminus.y , 0) - vec_h;
	vec3 vec_zplus = vec3(0, h_zplus.x + h_zplus.y,1) - vec_h;
	vec3 vec_zminus = vec3(0 , h_zminus.x + h_zminus.y, -1) - vec_h;

	vec3 norm_xplus = cross(vec_xplus, vec_zminus);
	vec3 norm_zminus = cross(vec_zminus, vec_xminus);
	vec3 norm_xminus = cross(vec_xminus, vec_zplus);
	vec3 norm_zplus = cross(vec_zplus, vec_xplus);

	vec3 tot_norm= normalize((norm_xplus+norm_zminus+norm_xminus+norm_zplus)/4);

	mat4 mdlMat = mdlMatrix;
	
	
	float water_depth = heights.x;
	if (water_depth < 0.002){
		water_depth = -0.15;
		}

    mdlMat[3][1] = heights.y + water_depth;

	gl_Position = projMatrix * camMatrix * mdlMat * vec4(inPosition, 1.0);

	mat3 normalMatrix = mat3(camMatrix * mdlMat);
	normalMatrix = inverse(normalMatrix);
	normalMatrix = transpose(normalMatrix);
	exNormal = normalMatrix * tot_norm;
	texCoord = inTexCoord;


}
