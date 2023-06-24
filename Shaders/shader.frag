#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

struct BaseLight
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	BaseLight base;
	vec3 direction;
};

struct PointLight
{
	BaseLight base;
	vec3 position;
	float exponent;
	float linear;
	float constant;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;

uniform Material material;

uniform vec3 eyePosition;

float CalcDirectionalShadowFactor(DirectionalLight light)
{
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float closest = texture(directionalShadowMap, projCoords.xy).r;
	float current = projCoords.z;

	float shadow = current > closest ? 1.0 : 0.0;

	return shadow;
}

vec4 CalcLightByDirection(BaseLight light, vec3 direction, float shadowFactor)
{
	vec4 ambientColour = vec4(light.colour, 1.0) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0);
	vec4 diffuseColour = vec4(light.colour, 1.0) * light.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.colour * material.specularIntensity * specularFactor, 1.0);
		}
	}

	return (ambientColour + (1.0 -shadowFactor) * (diffuseColour + specularColor));
}

vec4 CalcDirectionalLight()
{
	float shadowFactor = CalcDirectionalShadowFactor(directionalLight);

	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pointLight)
{
	vec3 direction = FragPos - pointLight.position;
	float dist = length(direction);
	direction = normalize(direction);

	vec4 colour = CalcLightByDirection(pointLight.base, direction, 0.0);
	float attenuation = pointLight.exponent * pow(dist, 2) + pointLight.linear * dist + pointLight.constant;

	return (colour / attenuation);
}

vec4 CalcSpotLight(SpotLight spotLight)
{
	vec3 rayDirection = normalize(FragPos - spotLight.base.position);
	float spotLightFactor = dot(rayDirection, spotLight.direction);

	if (spotLightFactor > spotLight.edge)
	{
		vec4 colour = CalcPointLight(spotLight.base);

		return colour * (1.0 - (1.0 - spotLightFactor) * (1.0 / (1.0 - spotLight.edge)));
	}
	else
	{
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);

	for (int i = 0; i < pointLightCount; i++)
	{
		totalColour += CalcPointLight(pointLights[i]);
	}

	return totalColour;
}

vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);

	for (int i = 0; i < spotLightCount; i++)
	{
		totalColour += CalcSpotLight(spotLights[i]);
	}

	return totalColour;
}

void main()
{
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();
	finalColour += CalcSpotLights();

	colour = texture(theTexture, TexCoord) * finalColour;
}