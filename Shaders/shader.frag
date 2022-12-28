#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;

out vec4 colour;

struct BaseLight
{
	vec3 colour;
	float ambientIntensity;
};

struct DirectionalLight 
{
	vec3 colour;
	vec3 direction;
	float diffuseIntensity;
};

uniform sampler2D theTexture;
uniform BaseLight baseLight;
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambientColour = vec4(baseLight.colour, 1.0) * baseLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0);
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0) * directionalLight.diffuseIntensity * diffuseFactor;

	colour = texture(theTexture, TexCoord) * (ambientColour + diffuseColour);
}