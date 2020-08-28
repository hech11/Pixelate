#shader vertex

#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;
layout(location = 3) in float aTexIndex;


uniform mat4 u_ViewProj = mat4(1.0f);

out vec4 v_Color;
out vec2 v_UV;
out float v_TexIndex;


void main() {
	v_Color = aColor;
	v_UV = aUV;
	v_TexIndex = aTexIndex;
	gl_Position = u_ViewProj * aPos;
}



#shader fragment

#version 330 core

layout(location = 0) out vec4 FragColor;


in vec4 v_Color;
in vec2 v_UV;
in float v_TexIndex;


uniform sampler2D u_Textures[gl_MaxCombinedTextureImageUnits];

#define GenTexCol(num) case num: texColor = texture(u_Textures[num], v_UV) * v_Color; break;
//TODO: Temp
void main() {

	vec4 texColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	switch (int(v_TexIndex)) {

		GenTexCol(0);
		GenTexCol(1);
		GenTexCol(2);
		GenTexCol(3);
		GenTexCol(4);
		GenTexCol(5);
		GenTexCol(6);
		GenTexCol(7);
		GenTexCol(8);
		GenTexCol(9);

		GenTexCol(10);
		GenTexCol(11);
		GenTexCol(12);
		GenTexCol(13);
		GenTexCol(14);
		GenTexCol(15);
		GenTexCol(16);
		GenTexCol(17);
		GenTexCol(18);
		GenTexCol(19);
		GenTexCol(20);

		GenTexCol(21);
		GenTexCol(22);
		GenTexCol(23);
		GenTexCol(24);
		GenTexCol(25);
		GenTexCol(26);
		GenTexCol(27);
		GenTexCol(28);
		GenTexCol(29);
		GenTexCol(30);

		GenTexCol(31);
		GenTexCol(32);
		GenTexCol(33);
		GenTexCol(34);
		GenTexCol(35);
		GenTexCol(36);
		GenTexCol(37);
		GenTexCol(38);
		GenTexCol(39);
		GenTexCol(40);

		GenTexCol(41);
		GenTexCol(42);
		GenTexCol(43);
		GenTexCol(44);
		GenTexCol(45);
		GenTexCol(46);
		GenTexCol(47);
		GenTexCol(48);
		GenTexCol(49);
		GenTexCol(50);

	}


	FragColor = texColor;
}
