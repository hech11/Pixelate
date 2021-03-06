#shader vertex

#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aUV;


uniform mat4 u_ViewProj = mat4(1.0f);

out vec2 v_uv;

void main() {
	gl_Position = u_ViewProj * aPos;
	v_uv = aUV;
}



#shader fragment

#version 330 core


out vec4 Color;
uniform float u_SceneCameraZoom = 1.0f;


in vec2 v_uv;

float grid(vec2 st, float res)
{
	vec2 grid = fract(st * res);
	return (step(res, grid.x) * step(res, grid.y));
}

void main() {
	vec2 grid_uv = v_uv.xy * 1000000.0f / 2.65f; // scale
	grid_uv.xy += 11.25f;
	float x = 1.0f - grid(grid_uv, 0.05f); // resolution
	Color.xyz = vec3(0.45) * x;
	float alpha = u_SceneCameraZoom;
	alpha = 1.0f / alpha;
	Color.a = x * alpha;


}
