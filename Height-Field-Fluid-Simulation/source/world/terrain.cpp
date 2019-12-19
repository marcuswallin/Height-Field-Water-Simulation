#pragma once
#include "terrain.h"

using namespace std;


Terrain::Terrain(char * ground_gen_file) {
	LoadTGATextureData(ground_gen_file, &terrain_texture);
	grid_size_x = terrain_texture.width;
	grid_size_z = terrain_texture.height;
	tex_to_vector();
	model = GenerateTerrain(this, false, 1);
}


void Terrain::init_program(const mat4& proj_mat, const mat4& wtv_mat) {
	program = loadShaders("source/shaders/terrain.vert", "source/shaders/terrain.frag");
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, wtv_mat.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, proj_mat.m);

}

void Terrain::draw(const mat4& cam_mat, const mat4& mtw_mat) {
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, cam_mat.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mtwMatrix"), 1, GL_TRUE, mtw_mat.m);

	glUniform1i(glGetUniformLocation(program, "tex"), 0);
	glUniform1i(glGetUniformLocation(program, "bigTex"), 4);
	glUniform1i(glGetUniformLocation(program, "gravelTex"), 5);
	glUniform1i(glGetUniformLocation(program, "sandTex"), 6);

	DrawModel(model, program, "inPosition", "inNormal", "inTexCoord");

}
void Terrain::tex_to_vector() {
	grid_array = new vec4[grid_size_x * grid_size_z];
	for (int z = 0; z < grid_size_z; z++)
		for (int x = 0; x < grid_size_x; x++){
			at(x,z)->x = z *20.0 / grid_size_z + 0.05 * terrain_texture.imageData[(x + z * terrain_texture.width) * (terrain_texture.bpp / 8)];
		}
}