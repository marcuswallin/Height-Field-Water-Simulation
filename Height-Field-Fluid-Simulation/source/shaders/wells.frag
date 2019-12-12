


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
	
    vec4 color = vec4(0.5,0.25,0.1, 1.0);

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

	diffuse = clamp(diffuse, 0, 0.5);

	//specular
	r = normalize(reflect(- lightView, normalize(exNormal)));
		
	
	specular = dot(r,v);
	if (specular > 0.0)
		specular = 1.0 * pow(specular, 50.0);

	specular = clamp(specular, 0, 0.5);

	shade =  diffuse + specular;
	color = vec4(shade,shade,shade,1.0);//*color;

    outColor = color;

}
