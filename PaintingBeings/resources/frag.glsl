uniform int u_colorR;
uniform int u_colorG;
uniform int u_colorB;

void main( )
{
	gl_FragColor =  vec4(u_colorR/255.0, u_colorG/255.0, u_colorB/255.0, 1.0);
}
