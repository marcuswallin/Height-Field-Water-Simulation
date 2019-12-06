#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
  //  vec4 col = texture(texUnit, outTexCoord);
    out_Color.r =3-outTexCoord.x*3;
    out_Color.g = 0;
    out_Color.b = 0;
    out_Color.a = 1.0;

}
