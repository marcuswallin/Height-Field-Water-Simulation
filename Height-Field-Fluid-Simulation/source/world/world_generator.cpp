#include "world_generator.h"


using namespace std;

//generates the terrain model based on texture data
//should change to height array data
Model* GenerateTerrain(HeightGrid* height_grid, bool generate_zero, int resolution)
{
	const int vertexCount = height_grid->grid_size_x * height_grid->grid_size_z;
	int triangleCount = (height_grid->grid_size_x - 1) * (height_grid->grid_size_z - 1) * 2;
	int x, z;

    GLfloat* vertexArray = new GLfloat[3*vertexCount];
	GLfloat* normalArray = new GLfloat[3 * vertexCount];
	GLfloat* texCoordArray = new GLfloat[2 * vertexCount];
	GLuint* indexArray = new GLuint[3 * triangleCount];



	for (x = 0; x < height_grid->grid_size_x; x++)
		for (z = 0; z < height_grid->grid_size_z; z++)
		{
		
			vertexArray[(x + z * height_grid->grid_size_x) * 3 + 0] = (float)x / resolution;
			if(!generate_zero)
				vertexArray[(x + z * height_grid->grid_size_x) * 3 + 1] = height_grid->at(x,z)->x;
			else
				vertexArray[(x + z * height_grid->grid_size_x) * 3 + 1] = 0;
			vertexArray[(x + z * height_grid->grid_size_x) * 3 + 2] = (float)z / resolution;


			texCoordArray[(x + z * height_grid->grid_size_x) * 2 + 0] = x;
			texCoordArray[(x + z * height_grid->grid_size_x) * 2 + 1] = z; 
		}
	for (x = 0; x < height_grid->grid_size_x; x++)
		for (z = 0; z < height_grid->grid_size_z; z++)
		{
			vec3 normal = calculateNormal(vertexArray, x, z, height_grid);

			normalArray[(x + z * height_grid->grid_size_x) * 3 + 0] = normal.x;
			normalArray[(x + z * height_grid->grid_size_x) * 3 + 1] = normal.y;
			normalArray[(x + z * height_grid->grid_size_x) * 3 + 2] = normal.z;
		}
	for (x = 0; x < height_grid->grid_size_x - 1; x++)
		for (z = 0; z < height_grid->grid_size_z - 1; z++)
		{
			// Triangle 1
			indexArray[(x + z * (height_grid->grid_size_x - 1)) * 6 + 0] = x + z * height_grid->grid_size_x;
			indexArray[(x + z * (height_grid->grid_size_x - 1)) * 6 + 1] = x + (z + 1) * height_grid->grid_size_x;
			indexArray[(x + z * (height_grid->grid_size_x - 1)) * 6 + 2] = x + 1 + z * height_grid->grid_size_x;
			// Triangle 2
			indexArray[(x + z * (height_grid->grid_size_x - 1)) * 6 + 3] = x + 1 + z * height_grid->grid_size_x;
			indexArray[(x + z * (height_grid->grid_size_x - 1)) * 6 + 4] = x + (z + 1) * height_grid->grid_size_x;
			indexArray[(x + z * (height_grid->grid_size_x - 1)) * 6 + 5] = x + 1 + (z + 1) * height_grid->grid_size_x;
		}


	// Create Model and upload to GPU:
	Model* model = LoadDataToModel(
		vertexArray,
		normalArray,
		texCoordArray,
		NULL,
		indexArray,
		vertexCount,
		triangleCount * 3);

	return model;
}



vec3 calculateNormal(GLfloat* vertexArray, int x, int z, const HeightGrid* height_grid)
{

	vec3 this_vertice = getVertex(vertexArray, x, z, height_grid);
	vec3 p1 = getVertex(vertexArray, x, (z - 1), height_grid);
	vec3 p2 = getVertex(vertexArray, (x + 1), (z - 1), height_grid);
	vec3 p3 = getVertex(vertexArray, x + 1, z, height_grid);
	vec3 p4 = getVertex(vertexArray, x, z + 1, height_grid);
	vec3 p5 = getVertex(vertexArray, x - 1, z + 1, height_grid);
	vec3 p6 = getVertex(vertexArray, x - 1, z, height_grid);

	vec3 v1 = SetVector(0.0f, 0.0f, 0.0f);
	vec3 v2 = SetVector(0.0f, 0.0f, 0.0f);
	vec3 v3 = SetVector(0.0f, 0.0f, 0.0f);
	vec3 v4 = SetVector(0.0f, 0.0f, 0.0f);
	vec3 v5 = SetVector(0.0f, 0.0f, 0.0f);
	vec3 v6 = SetVector(0.0f, 0.0f, 0.0f);

	if (!(p1 == v1)){
		v1 = VectorSub(p1, this_vertice);
	}
	if (!(p2 == v2)){
		v2 = VectorSub(p2, this_vertice);
	}
	if (!(p3 == v3)){
		v3 = VectorSub(p3, this_vertice);
	}
	if (!(p4 == v4)){
		v4 = VectorSub(p4, this_vertice);
	}
	if (!(p5 == v5)){
		v5 = VectorSub(p5, this_vertice);
	}
	if (!(p6 == v6)){
		v6 = VectorSub(p6, this_vertice);
	}

	vec3 normal = CrossProduct(v2, v1);
	normal = VectorAdd(normal, CrossProduct(v3, v2));
	normal = VectorAdd(normal, ScalarMult(CrossProduct(v4, v3), 2.0f));
	normal = VectorAdd(normal, CrossProduct(v5, v4));
	normal = VectorAdd(normal, CrossProduct(v6, v5));
	normal = VectorAdd(normal, ScalarMult(CrossProduct(v1, v6), 2.0f));

	normal = Normalize(normal);

	return normal;
}


vec3 getVertex(GLfloat* verticeArray, int x, int z, const HeightGrid* height_grid)
{
	
	if (x >= 0 && x < height_grid->grid_size_x && z >= 0 && z < height_grid->grid_size_z)
	{
		vec3 vertex;
		vertex.x = (GLfloat)verticeArray[(x + z * height_grid->grid_size_x) * 3 + 0];
		vertex.y = (GLfloat)verticeArray[(x + z * height_grid->grid_size_x) * 3 + 1];
		vertex.z = (GLfloat)verticeArray[(x + z * height_grid->grid_size_x) * 3 + 2];
		return vertex;
	}
	else
		return SetVector(0.0f, 0.0f, 0.0f);
}


