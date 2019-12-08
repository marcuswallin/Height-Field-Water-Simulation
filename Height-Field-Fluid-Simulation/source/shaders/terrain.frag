#version 150

out vec4 outColor;
uniform sampler2D tex;
uniform sampler2D bigTex;
uniform sampler2D gravelTex;


uniform mat4 camMatrix;

in vec3 exSurface;
in vec3 exNormal;
in vec2 texCoord;

void main(void)
{
	vec4 big_tex_color = texture(bigTex, vec2(exSurface.x/256, exSurface.z/256));
	float diffuse, shade;
	vec3 light = vec3(-1,1,-0.6);
	mat3 lightMatrix = mat3(camMatrix);
	lightMatrix = transpose(inverse(lightMatrix));
	vec3 lightView = mat3(lightMatrix) * light;

	diffuse = dot(normalize(exNormal), normalize( lightView));
	diffuse = clamp(diffuse, 0, 1);
	shade = diffuse;

	vec4 ground_c = vec4(0);
	if(exSurface.y < 1.0)
		ground_c = vec4(shade, shade, shade, 1.0) * texture(gravelTex, texCoord);
	else if(exSurface.y > 1.0)
		ground_c = vec4(shade, shade, shade, 1.0) * texture(tex, texCoord);
	
	vec4 big_ground = vec4(shade, shade, shade, 1.0) * vec4(0,0.7,0,1);//big_tex_color/2;
	ground_c.a = 1;
    outColor = ground_c;// + big_ground;

}
