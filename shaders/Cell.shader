#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aOffset;

uniform mat4 u_MVP;

uniform float u_maxDecay;

out vec3 vWorldPos;
out vec3 vNormal;

out float cellState;

void main()
{
	if (aOffset.w != 0.0)
	{
		vec3 worldPos = aPos + aOffset.xyz;
        vWorldPos = worldPos;
        vNormal = normalize(aPos);
        gl_Position = u_MVP * vec4(worldPos, 1.0);
	}
	else
	{
		gl_Position = vec4(9999, 9999, 99999, 1.0);
        vWorldPos = vec3(0.0);
        vNormal = vec3(0.0);
	}

	cellState = aOffset.w;
}

#shader fragment
#version 330 core

in vec3 vWorldPos;
in vec3 vNormal;
in float cellState;

out vec4 FragColor;

uniform float u_maxDecay;
vec3 aliveColor = vec3(0.337, 0.549, 0.451); 
vec3 decayColor = vec3(0.51, 0.196, 0.267);
vec3 midColor1 = vec3(0.376, 0.247, 0.431);
vec3 midColor2 = vec3(0.627, 0.71, 0.408);

vec3 getColor(float state)
{
    if (state == 1.0)
        return aliveColor;

    float t = clamp((state - 1.0) / (u_maxDecay - 1.0), 0.0, 1.0);

    if (t < 0.33)
    {
        float localT = t / 0.33;
        return mix(aliveColor, midColor1, localT);
    }
    else if (t < 0.66)
    {
        float localT = (t - 0.33) / 0.33;
        return mix(midColor1, midColor2, localT);
    }
    else
    {
        float localT = (t - 0.66) / 0.34; 
        return mix(midColor2, decayColor, localT);
    }
}

void main() 
{
    vec3 lightDir = normalize(vec3(1.0, 2.0, 1.0));
    float diffuse = max(dot(normalize(vNormal), lightDir), 0.0);

    float ambient = 0.5;
    float lighting = ambient + 0.7 * diffuse;

    FragColor = vec4(getColor(cellState) * lighting, 1.0);
}