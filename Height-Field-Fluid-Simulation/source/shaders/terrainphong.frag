#version 150

out vec4 outColor;
uniform sampler2D tex;
uniform mat4 camMatrix;

in vec3 exSurface;
in vec3 exNormal;
in vec2 texCoord;

void main(void)
{
	float diffuse, specular, shade;
	vec3 color = vec3(0);
	vec3 v = normalize(- exSurface);
	vec3 r;

	vec3 light = vec3(-1,1,-1);
    vec3 lightView;
	//float intensity = 0.5;

	//diffuse
	mat3 lightMatrix = mat3(camMatrix);
	lightMatrix = transpose(inverse(lightMatrix));
	lightView = mat3(lightMatrix) * light;

	diffuse = dot(normalize(exNormal), normalize( lightView));

	diffuse = clamp(diffuse, 0, 1);

	//specular
	r = normalize(reflect(- lightView, normalize(exNormal)));
		
	
	specular = dot(r,v);
	if (specular > 0.0)
		specular = 1.0 * pow(specular, 50.0);

	specular = clamp(specular, 0, 1);



	shade =   diffuse  + specular;
	color = vec3(shade);

    outColor = vec4(color, 1.0) * texture(tex, texCoord);

}
