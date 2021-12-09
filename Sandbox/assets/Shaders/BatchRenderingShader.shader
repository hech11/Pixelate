#shader vertex

#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;
layout(location = 3) in float aTexIndex;
layout(location = 4) in int aEntityID;


layout(binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 UV;
	float TexIndex;
};


layout(location = 0) out VertexOutput v_Output;
layout(location = 3) out int v_EntityID;


void main() {

	v_Output.Color = aColor;
	v_Output.UV = aUV;
	v_Output.TexIndex = aTexIndex;
	v_EntityID = aEntityID;


	gl_Position = u_ViewProjection * vec4(aPos, 1.0f);
}



#shader fragment

#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityColor;


struct VertexOutput
{
	vec4 Color;
	vec2 UV;
	float TexIndex;
};

layout(location = 0) in VertexOutput v_Output;
layout(location = 3) in flat int v_EntityID;

layout(binding = 0) uniform sampler2D u_Textures[32];


// This only supports up to 32 texture slots. Most GPU's can support more than this.
// This switch/case is done since modern AMD GPU's do not support sampler indexing.

void main() {

	switch (int(v_Output.TexIndex)) {

	case 0: FragColor = texture(u_Textures[0], v_Output.UV) * v_Output.Color; break;
	case 1: FragColor = texture(u_Textures[1], v_Output.UV) * v_Output.Color; break;
	case 2: FragColor = texture(u_Textures[2], v_Output.UV) * v_Output.Color; break;
	case 3: FragColor = texture(u_Textures[3], v_Output.UV) * v_Output.Color; break;
	case 4: FragColor = texture(u_Textures[4], v_Output.UV) * v_Output.Color; break;
	case 5: FragColor = texture(u_Textures[5], v_Output.UV) * v_Output.Color; break;
	case 6: FragColor = texture(u_Textures[6], v_Output.UV) * v_Output.Color; break;
	case 7: FragColor = texture(u_Textures[7], v_Output.UV) * v_Output.Color; break;
	case 8: FragColor = texture(u_Textures[8], v_Output.UV) * v_Output.Color; break;
	case 9: FragColor = texture(u_Textures[9], v_Output.UV) * v_Output.Color; break;

	case 10: FragColor = texture(u_Textures[10], v_Output.UV) * v_Output.Color; break;
	case 11: FragColor = texture(u_Textures[11], v_Output.UV) * v_Output.Color; break;
	case 12: FragColor = texture(u_Textures[12], v_Output.UV) * v_Output.Color; break;
	case 13: FragColor = texture(u_Textures[13], v_Output.UV) * v_Output.Color; break;
	case 14: FragColor = texture(u_Textures[14], v_Output.UV) * v_Output.Color; break;
	case 15: FragColor = texture(u_Textures[15], v_Output.UV) * v_Output.Color; break;
	case 16: FragColor = texture(u_Textures[16], v_Output.UV) * v_Output.Color; break;
	case 17: FragColor = texture(u_Textures[17], v_Output.UV) * v_Output.Color; break;
	case 18: FragColor = texture(u_Textures[18], v_Output.UV) * v_Output.Color; break;
	case 19: FragColor = texture(u_Textures[19], v_Output.UV) * v_Output.Color; break;
	case 20: FragColor = texture(u_Textures[20], v_Output.UV) * v_Output.Color; break;
	case 21: FragColor = texture(u_Textures[21], v_Output.UV) * v_Output.Color; break;

	case 22: FragColor = texture(u_Textures[22], v_Output.UV) * v_Output.Color; break;
	case 23: FragColor = texture(u_Textures[23], v_Output.UV) * v_Output.Color; break;
	case 24: FragColor = texture(u_Textures[24], v_Output.UV) * v_Output.Color; break;
	case 25: FragColor = texture(u_Textures[25], v_Output.UV) * v_Output.Color; break;
	case 26: FragColor = texture(u_Textures[26], v_Output.UV) * v_Output.Color; break;
	case 27: FragColor = texture(u_Textures[27], v_Output.UV) * v_Output.Color; break;
	case 28: FragColor = texture(u_Textures[28], v_Output.UV) * v_Output.Color; break;
	case 29: FragColor = texture(u_Textures[29], v_Output.UV) * v_Output.Color; break;
	case 30: FragColor = texture(u_Textures[30], v_Output.UV) * v_Output.Color; break;
	case 31: FragColor = texture(u_Textures[31], v_Output.UV) * v_Output.Color; break;
	}


	EntityColor = v_EntityID;
}
