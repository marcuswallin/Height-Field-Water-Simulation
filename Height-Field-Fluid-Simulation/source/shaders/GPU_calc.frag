#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
  //  vec4 col = texture(texUnit, outTexCoord);
    out_Color.r =1.0;
    out_Color.g = 3.0;
    out_Color.b = 0;
}
