#version 330

in vec4 vCol;
in vec2 TexCoord;

out vec4 colour;

struct BaseLight
{
	vec3 colour;
	float ambientIntensity;
};

uniform sampler2D theTexture;
uniform BaseLight baseLight;

void main()
{
	vec4 ambientColour = vec4(baseLight.colour, 1.0) * baseLight.ambientIntensity;

	colour = texture(theTexture, TexCoord) * ambientColour;
}