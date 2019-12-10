#version 150

out vec4 outColor;
uniform sampler2D waterHeight;
uniform sampler2D tex;
uniform mat4 camMatrix;

in vec3 exSurfaceG;
uniform int show_grid;
uniform int show_depth;



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
	vec4 color = vec4(shade, shade, shade, 1.0)*vec4(0.2,0.45,0.7,1);//texture(tex, texCoordG);
	//temporary workaround

	float v_dot = dot(normalize(exNormalG), v);
	float water_h = clamp(water_heightG, 1, 5);
	color.a = clamp((1-v_dot)*(1+water_heightG/4), 0.15*water_h, 0.99);
    outColor = color;
	if (show_grid == 1)
	{
		outColor = vec4(shade, shade, shade, 1.0)*texture(tex, texCoordG);
	}
	if(show_depth == 1)
	{
		outColor = vec4(color.a, color.a, color.a, 1.0);
	}
	

}
