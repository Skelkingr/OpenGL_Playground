#version 330

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;

out vec4 colour;

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

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
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
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform Material material;

uniform vec3 eyePosition;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float CalcDirectionalShadowFactor(DirectionalLight light)
{
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float current = projCoords.z;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(light.direction);

	float bias = max(0.05 * (1.0 - dot(Normal, light.direction)), 0.005);

	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += current - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9.0;

	if (projCoords.z > 1.0)
	{
		shadow = 0.0;
	}

	return shadow;
}

float CalcOmniShadowFactor(PointLight light, int shadowIndex)
{
	vec3 fragToLight = FragPos - light.position;
	float currentDepth = length(fragToLight);

	float shadow = 0.0;
	float bias = 0.05;
	int samples = 20;

	float viewDistance = length(eyePosition - FragPos);
	float diskRadius = (1.0 + (viewDistance / omniShadowMaps[shadowIndex].farPlane)) / 25.0;

	for (int i = 0; i < samples; i++)
	{
		float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= omniShadowMaps[shadowIndex].farPlane;

		if (currentDepth - bias > closestDepth)
		{
			shadow += 1.0;	
		}
	}

	shadow /= float(samples);
	return shadow;
};

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

vec4 CalcPointLight(PointLight pointLight, int shadowIndex)
{
	vec3 direction = FragPos - pointLight.position;
	float dist = length(direction);
	direction = normalize(direction);

	float shadowFactor = CalcOmniShadowFactor(pointLight, shadowIndex);

	vec4 colour = CalcLightByDirection(pointLight.base, direction, shadowFactor);
	float attenuation = pointLight.exponent * pow(dist, 2) + pointLight.linear * dist + pointLight.constant;

	return (colour / attenuation);
}

vec4 CalcSpotLight(SpotLight spotLight, int shadowIndex)
{
	vec3 rayDirection = normalize(FragPos - spotLight.base.position);
	float spotLightFactor = dot(rayDirection, spotLight.direction);

	if (spotLightFactor > spotLight.edge)
	{
		vec4 colour = CalcPointLight(spotLight.base, shadowIndex);

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
		totalColour += CalcPointLight(pointLights[i], i);
	}

	return totalColour;
}

vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);

	for (int i = 0; i < spotLightCount; i++)
	{
		totalColour += CalcSpotLight(spotLights[i], i + pointLightCount);
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