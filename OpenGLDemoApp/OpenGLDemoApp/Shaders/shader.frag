#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;

in vec3 FragPos;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct BaseLight
{
	vec3 color;
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
	float constant;
	float linear;
	float quadratic;
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

uniform sampler2D u_Texture; // this uniform samples from the texture unit that we set via glActiveTexture

uniform int u_pointLightCount;
uniform int u_spotLightCount;

uniform DirectionalLight u_directionalLight;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];
uniform Material u_material;

uniform vec3 u_eyePosition;

vec4 CaclulateLightByDirection(BaseLight light, vec3 direction)
{
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(u_eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		float specularFactor = dot(fragToEye, reflectedVertex);

		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, u_material.shininess);
			specularColor = vec4(light.color * u_material.specularIntensity * specularFactor, 1.0f);
		}
	}
	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalculateDirectionalLight()
{
	return CaclulateLightByDirection(u_directionalLight.base, u_directionalLight.direction);
}

vec4 CalculatePointLight(PointLight pLight)
{
	vec3 direction = FragPos - pLight.position;
	float fragmentDistance = length(direction);
	direction = normalize(direction);

	vec4 color = CaclulateLightByDirection(pLight.base, direction);
	float attenuation = pLight.quadratic * fragmentDistance * fragmentDistance +
						pLight.linear * fragmentDistance + 
						pLight.constant; 

	return (color / attenuation);
}

vec4 CalculateSpotLight(SpotLight spotLight)
{

	vec3 rayDirection = normalize(FragPos - spotLight.base.position);
	float spotLightFactor = dot(rayDirection, spotLight.direction);

	if(spotLightFactor > spotLight.edge)
	{
		vec4 color = CalculatePointLight(spotLight.base);

		return color * (1.0f - (1.0f - spotLightFactor)*(1.0f/(1.0f-spotLight.edge)));
	}
	else{
		return vec4(0,0,0,0);

	}
}

vec4 CalculatePointLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < u_pointLightCount; i++) 
	{
		totalColor += CalculatePointLight(u_pointLights[i]);
	}
	return totalColor;
}

vec4 CalculateSpotLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < u_spotLightCount; i++) 
	{
		totalColor += CalculateSpotLight(u_spotLights[i]);
	}
	return totalColor;
}

void main() {

	vec4 finalColor = CalculateDirectionalLight();
	finalColor += CalculatePointLights();
	finalColor += CalculateSpotLights();

	color = texture(u_Texture, TexCoord) * finalColor;
}