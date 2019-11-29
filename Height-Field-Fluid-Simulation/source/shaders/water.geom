#version 150
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 texCoord[3];

out vec2 texCoordG;
out vec3 exNormalG;

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
	 exNormalG = vec3(0,1,0);//avgNormal;
	 EmitVertex();
 }	
 EndPrimitive();
}