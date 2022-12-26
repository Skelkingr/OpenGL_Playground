#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;

out vec4 colour;

struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity; 
};

struct DiffuseLight
{
	vec3 direction;
	float diffuseIntensity;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform DiffuseLight diffuseLight;

void main()
{
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(diffuseLight.direction)), 0.0f);
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * diffuseLight.diffuseIntensity * diffuseFactor;

	colour = texture(theTexture, TexCoord) * (ambientColour + diffuseColour);
}