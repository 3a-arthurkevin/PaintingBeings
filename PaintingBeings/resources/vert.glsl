//#extension GL_ARB_draw_instanced	: enable
//#extension GL_EXT_gpu_shader4		: enable // Uncomment if ARB doesn't work
//#extension GL_EXT_draw_instanced	: enable // Uncomment if ARB doesn't work 

uniform int u_size;

//uniform float u_spacing;

uniform int u_grayScale;

uniform int u_xPosition;
uniform int u_yPosition;

uniform float u_random;

void main()
{	
	vec4 position = gl_Vertex;
	
	position.x -= ((u_size - u_xPosition) - (u_size/2.0) - cos(u_random));
	position.y += (u_size - u_yPosition - (u_size/2.0) + sin(u_random));
	position.z += (((u_grayScale/255.0) * u_size) - (u_size/2.0) /*+ tan(u_random)*/);
	
	gl_Position	= gl_ModelViewProjectionMatrix * position;
}
