//=============================================================================
//!
//!	@file	Brightness.vert
//!	@brief	輝度
//!
//!	@author	YKCH
//!
//=============================================================================
#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable

struct Input
{
	vec3 _position;		//!< 座標
};
struct Output
{
	vec2 _uv;
};
layout(location = 0) in  Input  input;
layout(location = 0) out Output output;

void main()
{
	vec4 position = vec4(input._position, 1);

	//	ポジションからuv作成
	output._uv = position.xy * 0.5 + 0.5;
	gl_Position = position;
}