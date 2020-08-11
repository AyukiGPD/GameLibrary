//=============================================================================
//!
//!	@file	ToneMapping.frag
//!	@brief	トーンマッピング
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


layout(location = 0) uniform mediump float exposureBias = 3.0f;

layout(location = 10) uniform sampler2D gTexture;

layout(location = 0) in		 Input	input;
layout(location = 0) out	 vec4	outColor;


const float A = 0.15;
const float B = 0.50;
const float C = 0.10;
const float D = 0.20;
const float E = 0.02;
const float F = 0.30;
const float W = 11.2;

vec3 Uncharted2Tonemap(float val)
{
	vec3 x = vec3(val,val,val);
	return ((x*(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E / F;
}
vec3 Uncharted2Tonemap(vec3 val)
{
	vec3 x = vec3(val);
	return ((x*(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E / F;
}


void main()
{
	vec2 uv = input._uv;
	vec3 texColor = texture2D(gTexture,uv).rgb;

	//	FilmicToneMapping
	vec3 curr = Uncharted2Tonemap(exposureBias*texColor);
	vec3 whiteScale = 1.0f / Uncharted2Tonemap(W);
	vec3 color = curr*whiteScale;
	
	outColor = vec4(color, 1.0);

}
