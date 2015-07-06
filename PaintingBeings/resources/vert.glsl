uniform int u_size;

uniform float u_spacing;

uniform int u_grayScale;

uniform int u_xPosition;
uniform int u_yPosition;

uniform float u_random;

uniform float u_time;
uniform float u_height;
uniform float u_angleMax;

vec4 getCubePosition()
{
	vec4 position = gl_Vertex;
	
	position.x -= (((u_size - u_xPosition) - (u_size/2.0)) * u_spacing);
	position.y += ((u_size - u_yPosition - (u_size/2.0)) * u_spacing);
	position.z += ((((u_grayScale/255.0) * u_size) - (u_size/2.0)) * u_spacing);
	
	return position;
}

vec4 getTwistPoint(vec4 pos, float ang)
{
	float sinAng = sin(ang);
	float cosAng = cos(ang);
	
	vec4 newPos;
	newPos.x = (pos.x * cosAng) - (pos.z * sinAng);
	newPos.z = (pos.x * sinAng) + (pos.z * cosAng);
	
	newPos.y = pos.y;
	newPos.w = pos.w;

	return newPos;
}

void setVerticesPosition()
{
	float angleDeg = u_angleMax * sin(u_time /*+ (u_random/100.0f)*/);
	float angleRad = angleDeg * 3.14159 / 180.0;
	
	float angle = ((u_height * 0.5) + gl_Vertex.y)/(u_height * angleRad);
	
	vec4 position = getTwistPoint(gl_Vertex, angle) + getCubePosition();

	gl_Position += (gl_ModelViewProjectionMatrix * position);
}

void main()
{	
	setVerticesPosition();
}
