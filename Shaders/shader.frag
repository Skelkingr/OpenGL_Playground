#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

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

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform sampler2D theTexture;

uniform BaseLight baseLight;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

void main()
{
	vec4 ambientColour = vec4(baseLight.colour, 1.0) * baseLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0);
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0) * directionalLight.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0);
		}
	}

	colour = texture(theTexture, TexCoord) * (ambientColour + diffuseColour + specularColor);
}