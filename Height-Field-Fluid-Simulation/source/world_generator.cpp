#include "world_generator.h"






Model* GenerateTerrain(TextureData* tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width - 1) * (tex->height - 1) * 2;
	int x, z;
	GLfloat* vertexArray;

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

			vec3 normal = SetVector(0, 0, 1);//calculateNormal(vertexArray, x, z);

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
