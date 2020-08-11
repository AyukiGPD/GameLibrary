//=============================================================================
//!
//!	@file	Emissive.frag
//!	@brief	”­Œõ
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
};

layout(location = 10) uniform sampler2D	texEmissiveColor;
layout(location = 11) uniform sampler2D	texDiffuse;

layout(location = 0) in		 Input	input;
layout(location = 0) out	 vec3	outColor;

void main()
{
	vec2 uv = input._uv;
	outColor = texture2D(texDiffuse,uv).rgb + texture2D(texEmissiveColor,uv).rgb;

}
