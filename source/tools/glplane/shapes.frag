#version 330
uniform vec3 Ambient = vec3(1.0);
uniform vec3 LightColor = vec3(1.0);
uniform vec3 LightDirection = vec3(1.00,0.0,1.0);
uniform vec3 HalfVector = vec3(0,0,1.0);
uniform float Shininess = 1.1;
uniform float Strength = 0.2;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

in vec4 exColor;
in vec3 exNormal;
in vec2 UV;
out vec4 fColor;

void main(void)
{
	vec4 Color;
	float diffuse = max(0.0, dot(exNormal, LightDirection));
	float specular = max(0.0, dot(exNormal, HalfVector));
	if (diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess);  // sharpen the highlight
	vec3 scatteredLight = Ambient + LightColor * diffuse;
	vec3 reflectedLight = LightColor * specular * Strength;
	
	Color = exColor;
//	if (UV.x < -0.5 || UV.y < -0.5)
//		Color = exColor;
//	else
//		Color = texture( myTextureSampler, UV ).rgba;
	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));
	fColor = vec4(rgb, Color.a);
}
