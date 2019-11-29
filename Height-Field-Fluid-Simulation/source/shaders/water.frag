#version 150

out vec4 outColor;
uniform sampler2D waterHeight;
uniform sampler2D tex;
uniform mat4 camMatrix;

in vec3 exSurface;
in vec3 exNormal;
in vec2 texCoord;

void main(void)
{
    vec4 pos = gl_FragCoord;



	float diffuse, shade;
	vec3 light = vec3(-1,1,-1);
	mat3 lightMatrix = mat3(camMatrix);
	lightMatrix = transpose(inverse(lightMatrix));
	vec3 lightView = mat3(lightMatrix) * light;

	diffuse = dot(normalize(exNormal), normalize( lightView));
	diffuse = clamp(diffuse, 0, 1);
	shade = diffuse;

    //outColor =  shade*vec4(0.1,0.1,1, 1.0);
	
    outColor = texture(waterHeight, texCoord)/10;
    
}
