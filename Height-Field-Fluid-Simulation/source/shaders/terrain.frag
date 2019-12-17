#version 150

out vec4 outColor;
uniform sampler2D tex;
uniform sampler2D bigTex;
uniform sampler2D gravelTex;
uniform sampler2D sandTex;

uniform mat4 camMatrix;

in vec3 exSurface;
in vec3 exNormal;
in vec2 texCoord;

void main(void)
{

	float diffuse, shade;
	vec3 light = vec3(-1,1,-0.6);
	mat3 lightMatrix = mat3(camMatrix);
	lightMatrix = transpose(inverse(lightMatrix));
	vec3 lightView = mat3(lightMatrix) * light;

	diffuse = dot(normalize(exNormal), normalize( lightView));
	diffuse = clamp(diffuse, 0, 1);
	shade = diffuse;

	vec4 gravel = texture(gravelTex, texCoord);
	vec4 grass = texture(tex, texCoord);
	vec4 sand = texture(sandTex, texCoord);
	vec4 big_tex_color = texture(bigTex, vec2(exSurface.x/256, exSurface.z/256));
	vec4 big_norm = vec4(normalize(vec3(big_tex_color)), 1);
	outColor = (big_norm.r +0.1) * sand;
	outColor += (big_norm.g + 0.1) * grass;
	outColor += (big_norm.b + 0.1) * gravel;
	outColor *=  vec4(shade, shade, shade, 1.0);

	outColor.a = 1;
}
