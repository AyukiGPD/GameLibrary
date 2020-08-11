//=============================================================================
//!
//!	@file	Standard2D.frag
//!	@brief	2Dシェーダー
//!
//!	@author	YKCH
//!
//=============================================================================
#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable

struct Input
{
	vec2	_uv;
	vec4	_color;
};

layout(location = 10) uniform sampler2D gTexture;

layout(location = 0) in		 Input	input;
layout(location = 0) out	 vec4	outColor;

void main()
{
	vec4 color   = input._color;
	vec4 diffuse = texture2D(gTexture, input._uv);
//	diffuse.a = 1;
	outColor = color * diffuse;
}
