//=============================================================================
//!
//!	@file	Standard2D.vert
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
	vec3	_position;
	vec2	_uv;
	vec4	_color;
};

struct Output
{
	vec2	_uv;
	vec4	_color;
};

layout(location = 0) in  Input		input;
layout(location = 0) out Output		output;

void main()
{
	output._uv    = input._uv;
	output._color = input._color;
	gl_Position   = vec4(input._position,1);
}