#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_height;

uniform int grid_x;
uniform int grid_z;
uniform int resolution;

int grid_res_x = grid_x / resolution;
int grid_res_z = grid_z / resolution;

float tex_x_plus = outTexCoord.s + 1.0 / grid_x + 0.001;
float tex_z_plus = outTexCoord.t + 1.0 / grid_z + 0.001;
float tex_x_minus = outTexCoord.s - 1.0 / grid_x + 0.001;
float tex_z_minus = outTexCoord.t - 1.0 / grid_z + 0.001;
vec4 h = texture(texUnit, outTexCoord);
vec4 h_xplus = texture(texUnit, vec2(tex_x_plus, outTexCoord.t));
vec4 h_xminus = texture(texUnit, vec2(tex_x_minus, outTexCoord.t));
vec4 h_zplus = texture(texUnit, vec2(outTexCoord.s, tex_z_plus));
vec4 h_zminus = texture(texUnit, vec2(outTexCoord.s, tex_z_minus));



float get_hbar_z(int x_offset, int z_offset)
{
	float hbar = h.x;
//	if (z_offset*get_velocity(x, z, x_offset, z_offset)->z <= 0 && 
//		z + z_offset < grid_size_z && z + z_offset >= 0) {
//		hbar = at(x, z+z_offset)->x;
//	}
	return hbar;
}




float get_hbar_x(int x_offset, int z_offset)
{   
	
	float hbar = h.x;
//	if (x_offset*get_velocity(x, z, x_offset, z_offset)->x <= 0 &&
	//	x+x_offset < grid_size_x && x+x_offset >=0) {
	//	hbar = at(x + x_offset, z)->x;
	//}
	return hbar;
}



float get_height_derivative() {
//	float hbarx_plus, hbarx_minus, hbarz_plus, hbarz_minus;
		
	//test eq.5 
	float hbarx_plus = get_hbar_x(1, 0);
	float hbarx_minus = get_hbar_x(-1, 0);
	float hbarz_plus = get_hbar_z(0, 1);
	float hbarz_minus = get_hbar_z(0, -1);
	
//	float h_avg_max = beta * 1 / (resolution * gravity * deltat);

//	float h_adj = max((float)0,
	//	(hbarx_plus + hbarx_minus + hbarz_plus + hbarz_minus) / 4 - h_avg_max);

	//does low difference to height increase
	//hbarx_plus -= h_adj;
//	hbarx_minus -= h_adj;
//	hbarz_plus -= h_adj;
//	hbarz_minus -= h_adj;

	//float dhdt = -(
		//(hbarx_plus * get_velocity(x, z, 1, 0)->x - 
		//hbarx_minus * get_velocity(x, z, -1, 0)->x) + 
		//(hbarz_plus * get_velocity(x, z, 0, 1)->z -
		//hbarz_minus * get_velocity(x, z, 0, -1)->z)) * resolution;
	//float friction_f = friction_force(dhdt);
	return 0.01;
}



void main(void)
{ 

	float dhdt = get_height_derivative();
	out_height.r = h.x+  0.01;
    out_height.g = h.g;
    out_height.b = 0;
    out_height.a = 1.0;
}



//vec4* WaterMass::get_velocity(int x, int z, int x_offset, int z_offset)
//{
//	int tmp_x = x_offset;
//	if (x_offset == -1) 
//		tmp_x = 0;
//	int x_vel = x + tmp_x;
	

//	if (z_offset == -1) 
//		z_offset = 0;
//	int z_vel = (z + z_offset) * 2;
//	if (x_offset != 0)
//		z_vel += 1;

//	return velocities.at(x_vel, z_vel);
//}