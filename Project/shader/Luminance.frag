//=============================================================================
//!
//!	@file	Luminance.frag
//!	@brief	•½‹Ï‹P“x
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
};

layout(location = 10) uniform sampler2D gTexture;

layout(location = 0) in		 Input	input;
layout(location = 0) out	 vec4	outColor;

const mediump vec3 luminance = vec3(0.27, 0.67, 0.06);

void main()
{
	vec2 uv = input._uv;
	vec3 color = texture(gTexture, uv).rgb * 4.0;
	outColor.r = dot(color, luminance);
}
