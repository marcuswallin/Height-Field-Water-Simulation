#include "world_generator.h"






Model* GenerateTerrain(TextureData* tex)
{
	const int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width - 1) * (tex->height - 1) * 2;
	int x, z;
	GLfloat* vertexArray;

	//vertexArray = new GLfloat[vertexCount];//(GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
	vertexArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);

	GLfloat* normalArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat* texCoordArray = (GLfloat*)malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint* indexArray = (GLuint*)malloc(sizeof(GLuint) * triangleCount * 3);


	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width) * 3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width) * 3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] / 20.0;
			vertexArray[(x + z * tex->width) * 3 + 2] = z / 1.0;
			// Normal vectors. You need to calculate these.


			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width) * 2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width) * 2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{

			vec3 normal = calculateNormal(vertexArray, x, z, tex);

			normalArray[(x + z * tex->width) * 3 + 0] = normal.x;
			normalArray[(x + z * tex->width) * 3 + 1] = normal.y;
			normalArray[(x + z * tex->width) * 3 + 2] = normal.z;
		}
	for (x = 0; x < tex->width - 1; x++)
		for (z = 0; z < tex->height - 1; z++)
		{
			// Triangle 1
			indexArray[(x + z * (tex->width - 1)) * 6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 1] = x + (z + 1) * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 2] = x + 1 + z * tex->width;
			// Triangle 2
			indexArray[(x + z * (tex->width - 1)) * 6 + 3] = x + 1 + z * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 4] = x + (z + 1) * tex->width;
			indexArray[(x + z * (tex->width - 1)) * 6 + 5] = x + 1 + (z + 1) * tex->width;
		}

	// End of terrain generation

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



vec3 calculateNormal(GLfloat* vertexArray, int x, int z, TextureData* tex)
{

	vec3 this_vertice = getVertex(vertexArray, x, z, tex);
	vec3 p1 = getVertex(vertexArray, x, (z - 1), tex);
	vec3 p2 = getVertex(vertexArray, (x + 1), (z - 1),  tex);
	vec3 p3 = getVertex(vertexArray, x + 1, z, tex);
	vec3 p4 = getVertex(vertexArray, x, z + 1,  tex);
	vec3 p5 = getVertex(vertexArray, x - 1, z + 1, tex);
	vec3 p6 = getVertex(vertexArray, x - 1, z, tex);

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


vec3 getVertex(GLfloat* verticeArray, int x, int z, TextureData* tex)
{
	
	if (x >= 0 && x < tex->width && z >= 0 && z < tex->height)
	{
		vec3 vertex;
		vertex.x = (GLfloat)verticeArray[(x + z * tex->width) * 3 + 0];
		vertex.y = (GLfloat)verticeArray[(x + z * tex->width) * 3 + 1];
		vertex.z = (GLfloat)verticeArray[(x + z * tex->width) * 3 + 2];
		return vertex;
	}
	else
		return SetVector(0.0f, 0.0f, 0.0f);
}


