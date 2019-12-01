#version 150
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 texCoord[3];
in vec3 exNormal[3];
in vec3 exSurface[3];
in float water_height[3];

out float water_heightG;
out vec2 texCoordG;
out vec3 exNormalG;
out vec3 exSurfaceG;

void main()
{	
// vec4 middleOfTriangle = vec4(0.0);
 //vec3 avgNormal = vec3(0.0);

// for(int i = 0; i < gl_in.length(); i++)
 //{	
// avgNormal += exNormal[i];
// }	
// middleOfTriangle /= gl_in.length();
 //avgNormal /= gl_in.length();
 //avgNormal = normalize(avgNormal);
 for(int i = 0; i < gl_in.length(); i++)
 {	
	 gl_Position = gl_in[i].gl_Position;
	 texCoordG = texCoord[i];
	 exNormalG = exNormal[i];//avgNormal;
	 exSurfaceG = exSurface[i];
	 water_heightG = water_height[i];
	 EmitVertex();
 }	
 EndPrimitive();
}