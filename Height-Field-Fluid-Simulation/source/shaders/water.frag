#version 150

out vec4 outColor;
uniform sampler2D waterHeight;
uniform sampler2D tex;
uniform mat4 camMatrix;

in vec3 exSurfaceG;
in vec3 exNormalG;
in vec2 texCoordG;
in float water_heightG;

void main(void)
{
    vec4 pos = gl_FragCoord;

	float diffuse, shade;
	vec3 light = vec3(-1,1,-0.6);
	mat3 lightMatrix = mat3(camMatrix);
	lightMatrix = transpose(inverse(lightMatrix));
	vec3 lightView = mat3(lightMatrix) * light;

	diffuse = dot(normalize(exNormalG), normalize( lightView));
	diffuse = clamp(diffuse, 0, 1);

	//specular
	vec3 r = normalize(reflect(- lightView, normalize(exNormalG)));
		
	vec3 v = normalize(- exSurfaceG);
	float specular = dot(r,v);
	if (specular > 0.0)
		specular = 1.0 * pow(specular, 250.0);

	specular = clamp(specular, 0, 1);


	shade = diffuse + specular;
	// vec4(water_heightG);
	vec4 color = vec4(shade, shade, shade, 1.0)*texture(tex, texCoordG);
	//temporary workaround

	color.a = clamp(water_heightG, 0.1, 0.99);
    outColor = color;
}
