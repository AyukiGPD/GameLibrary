//=============================================================================
//!
//!	@file	Brightness.frag
//!	@brief	‹P“x
//!
//!	@author	YKCH
//!
//=============================================================================
#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable

struct Input
{
	vec2	_uv;		//!< UV
	vec4	_color;		//!< F
};

layout(location = 10) uniform sampler2D gTexture;

layout(location = 0) in		 Input	input;
layout(location = 0) out	 vec4	outColor;

void main()
{
	vec2 uv = input._uv;
	vec3 texColor = texture2D(gTexture,uv).rgb * 0.1f;
	outColor = vec4(texColor,1.0);

}
